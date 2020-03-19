import re
import argparse
import logging
import os.path

logger = logging.getLogger(__name__)
FORMAT = '%(asctime)-15s:%(levelname)s: %(message)s'
logging.basicConfig(format=FORMAT)

template_c = """#include "{HEADER_FILENAME}"

U8 {SHORTNAME}_number_bursts_u8{BUFFER_ARRAY};
{SHORTNAME}_buffer_type {SHORTNAME}_buffer{BUFFER_ARRAY};
    
void copy_{SHORTNAME}({TYPE_NAME}* p_result_s, {SHORTNAME}_buffer_type* p_buffer_s, U8 buffer_index)
{{
{COPY_BUFFER_INSTRUCTIONS}
}}
"""

template_h = """#ifndef {FILENAME_H}
#define {FILENAME_H}

#ifdef __cplusplus
extern "C" {{
#endif

#include "FOUNDATION/FOUNDATION.H"
{INCLUDES}

typedef {BUFFER_TYPE_DECLARATION} {SHORTNAME}_buffer_type;

extern U8 {SHORTNAME}_number_bursts_u8{BUFFER_ARRAY};
extern {SHORTNAME}_buffer_type {SHORTNAME}_buffer{BUFFER_ARRAY};
void copy_{SHORTNAME}({TYPE_NAME}* p_result_s, {SHORTNAME}_buffer_type* p_buffer_s, U8 buffer_index);

#ifdef __cplusplus
}}
#endif

#endif /* {FILENAME_H} */"""

def split_by_unmatched_brace(s):
    """This function splits a string into two parts.
       The divider is the first unmatched closing brace. """
    brace_count = 1
    for i,c in enumerate(s):
        if c == '{':
            brace_count += 1
        elif c == '}':
            brace_count -= 1
            if brace_count == 0:
                return s[:i].strip(),s[i+1:].strip()
    raise ValueError('no matching brace')

class c_type():
    """ This class encapsulates a generic c-type.
        It contains methods to generate a type declaration and a copy instruction 
    """
    _ano_idx = 0;  # anonymous types need a tag name, the next anonymous type will be called t<_ano_idx>
    _lookup_table = dict()

    @classmethod
    def get_by_name(cls, name):
        """This function returns and creates a single c_type object for each name
        """
        try:
            # Try to return already created object
            return cls._lookup_table[name]
        except KeyError:
            if name.startswith('typedef '): # typedef struct <tag> <name>
                # in this case the declarations looks like:
                #     typedef struct <tag> <name>
                # We create a new type "struct <tag>"
                # and point from <name> to that new type."""
                x,name = name.rsplit(maxsplit=1) # typedef struct tag, name
                _,typename = x.split(maxsplit=1) # typedef, struct name
                new_type = cls.get_by_name(typename)
            elif name.startswith('struct'):
                # in this case the declaration looks like:
                #     struct <tag> [<some_identifier>] """
                name = ' '.join(name.split()[:2])
                new_type = struct(name)
            else:
                new_type = c_type(name)
            cls._lookup_table[name] = new_type
            return cls._lookup_table[name] 
      
    @classmethod
    def parse(cls, c_declaration):
        """ This function parses a file listing containing c type declarations.
            It returns a list of individual c declaration strings.
            It creates a lookup table for types (by calling cls.get_by_name)
            It calls itself recursively in order to map structs."""
        result = []
        
        # use while-loop to avoid tail-recursion
        while c_declaration:
            m = re.match('[ ]*(.*?)[ ]*([;{])[ ]*(.*)', c_declaration)
            if m:           
                pre, divider, remainder = m.groups() # <pre> <divider> <remainder>, divider is the first ; or {
        
                if divider == '{':
                    # pre { block_contents } member_or_typedef_name; remaining_declarations
                    block_contents, remainder = split_by_unmatched_brace(remainder)
                    member_or_typedef_name,remaining_declarations = [x.strip() for x in remainder.split(';',1)]
                                       
                    # if struct or enum in anonymous, we add a tag name here
                    L = pre.split()
                    if (L[0] == 'typedef' and L[1] in {'struct','enum'} and len(L)<3) or (L[0] in {'struct','enum'} and len(L)<2):
                            L += [c_type.get_anonymous_name()]
    
                    # complete declaration = <pre> <member_or_typedef_name>
                    this_declaration = ' '.join(L+[member_or_typedef_name]).strip()
                    logger.debug('Parsing {}: {}'.format(this_declaration, block_contents))                    
                    
                    # save type to lookup table and parse children
                    this_type = cls.get_by_name(this_declaration)
                    this_type.add_children(cls.parse(block_contents))
                    
                    # add this declaration to result and repeat for remaining_declarations
                    result += [this_declaration]
                    
                    c_declaration = remaining_declarations
                else:
                    # pre;remainder
                    # add pre to result and repeat for remainder
                    result += [pre]
                    c_declaration = remainder
            else:
                result+=[c_declaration]
                c_declaration = ''
        return result
    
    @classmethod
    def get_anonymous_name(cls):
        cls._ano_idx+=1
        return 't{:0>3d}'.format(cls._ano_idx-1)    
    
    def add_children(self, declarations):
        """ Adds children to this c-type
            Note: generic c types have no children. This method can be overwritten in subclasses (struct)
        """
        pass
    
    def __init__(self, name):
        self.name = name
        
    def generate_declaration(self, indent_string='    ',index_string=''):
        """ Generates a c-declaration for this c-type 
        """
        return indent_string + self.name
        
    def generate_copy_instruction(self, left_name ,right_name, indent_string='',index_string=''):
        """ Generates a copy instruction for this c_type
            
            Sample input: generate_copy_instruction(left_name="left_name", right_name="right_name", indent_string="    ", index_string="[i]")
            Returns: "    left_name = right_name[i];"
        """
        return indent_string+left_name+' = '+right_name+index_string+';'

class struct(c_type):
    """ This class encapsulates a c-struct.
        It contains methods to generate a type declaration and a copy instruction 
    """
    def __init__(self, name):
        super().__init__(name)
        self.members = []
    def add_children(self, declarations):
        """ Adds children to this c-type. 
            Finds the correct c_type instance and creates a corresponding struct member.
            
            Args:
                declarations (list of str): a list of c declarations e.g. ['F32 abc', 'struct some_tag s']
        """
        for declaration in declarations:
            membertype, membername  = declaration.rsplit(' ',1)
            child_type = c_type.get_by_name(membertype)
            self.members.append(structmember(membername, child_type))
            
    def generate_declaration(self, indent_string='',index_string=''):
        """ Generates the c declaration for this struct.
            Calls the generate_declaration method of its members to generate a joint declaration.        
            
            Note: Fails if no declaration of the struct type has been found.
        """
        if not self.members:
            logger.error('Type declaration for {name} not found. Aborting.'.format(name=self.name))
            exit(-1)
        return '{____}{name}_BUFFER\n{____}{{\n{member_string}\n{____}}}'.format(____=indent_string,name=self.name,member_string='\n'.join([m.generate_declaration(indent_string+'    ',index_string) for m in self.members]))

    def generate_copy_instruction(self, left_name='', right_name='', indent_string='',index_string=''):
        """ Generates the copy instruction for this struct.
            Calls the generate_copy_instruction method of its members to generate a joint copy instruction.        
            
            Note: Fails if no declaration of the struct type has been found.
        """
        if not self.members:
            logger.error('Type declaration for {name} not found. Aborting.'.format(name=self.name))
            exit(-1)
        return '\n'.join(m.generate_copy_instruction(left_name, right_name, indent_string,index_string) for m in self.members)


    
class structmember:
    """ This class encapsulates a c-struct-member.
        It contains methods to generate a type declaration and a copy instruction 
    """
    
    array_idx = []
    def __init__(self, _name, _type):
        self.name, *self.array = [x.strip('[]') for x in _name.split('[')]
        self.type = _type
    
    def generate_copy_instruction(self, left_name='', right_name='', indent_string='', index_string=''):
        """This function generates c-code to copy a struct member 
           from a array of structs buffer to the base element
        """
        result = ''
        this_array_idx = []
        
        for a in self.array: # handle arrays (indent, for-loops)
            next_array_idx = len(self.__class__.array_idx)
            self.__class__.array_idx.append(next_array_idx)
            this_array_idx.append(next_array_idx)
            result += '{____}for (int i{array_idx} = 0; i{array_idx} < {array_max}; i{array_idx}++)\n{____}{{\n'.format(____=indent_string,array_idx=next_array_idx,array_max=a)
            indent_string+='    '
        array_string = ''.join('[i{}]'.format(a) for a in this_array_idx)
        result+=self.type.generate_copy_instruction(left_name+'.'+self.name+array_string,right_name+'.'+self.name+array_string,indent_string,index_string)
    
        for a in self.array: # handle arrays (indent, for-loops)
            indent_string = indent_string[:-4]
            result+='\n'+indent_string+'}\n'
            
            self.__class__.array_idx.pop()
        return result
                
    def generate_declaration(self, indent_string='    ', index_string=''):
        """ Generates the declaration of this struct member
        """
        array_string = ''.join('['+a+']' for a in self.array)
        newl_string = '\n' if type(self.type) == struct else ''
        return self.type.generate_declaration(indent_string,index_string)+' '+ self.name + array_string + (index_string if type(self.type) is not struct else '') +';' + newl_string
    
def generate_cpp(header_filename, type_name, buffer_array):
    """ Generates the content of a source file containing the declarations of the buffer
        Args:
            header_filename (str): the filename of header file
            type_name (str): the name of the C-Type to be turned into a struct
    """

    instructions = c_type.get_by_name(type_name).generate_copy_instruction(index_string='[buffer_index]',right_name='(*p_buffer_s)', left_name="(*p_result_s)",indent_string='    ')
    
    return template_c.format(HEADER_FILENAME=header_filename.split('/')[-1], SHORTNAME=type_name.split()[-1].lower(), COPY_BUFFER_INSTRUCTIONS = instructions, TYPE_NAME=type_name, BUFFER_ARRAY = buffer_array)

def generate_includes(type_filenames):
    return '\n'.join(['#include "{}"'.format(f) for f in type_filenames])

def generate_h(header_filename, type_name, buffer_size, type_filenames, buffer_array):
    """ Generates the content of a header file containing the declarations of the buffer
        Args:
            header_filename (str): the filename of header file
            type_name (str): the name of the C-Type to be turned into a struct
            buffer_size (str): the size of the newly created buffer
            type_filenames (str): the filename of the header including the original type
    """

    declarations = c_type.get_by_name(type_name).generate_declaration(index_string='[{}]'.format(buffer_size))

    return template_h.format(
        FILENAME_H              = header_filename.split('/')[-1].replace('.','_').upper(),
        INCLUDES                = generate_includes(type_filenames),
        SHORTNAME               = type_name.split()[-1].lower(),
        BUFFER_TYPE_DECLARATION = declarations,
        TYPE_NAME               = type_name,
        BUFFER_ARRAY            = buffer_array)

def generate_files(type_name, buffer_size, type_filenames, target_header_filename, target_source_filename, mainapp_path, ctype_dimension):
    """ Generates a header and a cpp file that contain a buffer and the corresponding functions for a given type and a given size.
        Args:
            type_name (str): C-Type (typedef or struct) that shall be turned into a buffer.
            buffer_size (str): Size of the generated buffer
            type_filenames (list of str): List of C-Headerfiles that contain the necessary declarations of <type_name> and all sub-types
            target_header_filename (str): the name of the generated header file
            target_source_filename (str): the name of the generated cpp file
    """
    lines = []
    for filename in (os.path.join(mainapp_path, type_filename) for type_filename in type_filenames):    
        logger.info("Reading {}".format(filename))
        try:
            with open(filename,'r') as f:
                lines += f.readlines()
        except IOError as e:
            logger.error(e)
            exit(-1)

    declarations = ' '.join(re.sub('(//.*)|(#.*)','', re.sub('/\*.*?\*/','',line.strip())) for line in lines) # remove inline comments and then lines comments and newlines
    declarations = re.sub('/\*.*?\*/','',declarations) # remove multiline comments
    declarations = ' '.join(declarations.split()) # normalize whitespaces
    logger.info("Parsing declarations")
    c_type.parse(declarations)
    buffer_array = '[{}]'.format(ctype_dimension) if ctype_dimension > 1 else ''
        
    logger.info("Generating header file")
    code_h = generate_h(target_header_filename, type_name, buffer_size, type_filenames, buffer_array)
    logger.info("Generating cpp file")
    code_cpp = generate_cpp(target_header_filename, type_name, buffer_array)
    
    if code_h and code_cpp:
        logger.info("Writing output")
        try:
            with open(target_header_filename,'w') as header_file:
                print(code_h,file=header_file)
            with open(target_source_filename,'w') as source_file:
                print(code_cpp,file=source_file)
        except IOError as e:
            logger.error(e)
            exit(-1)

if __name__ == '__main__':
    import sys
    
    # setup command line interface
    parser = argparse.ArgumentParser(description='Generate a buffer (structs of arrays) from a given structure type.')
    parser.add_argument('-v', '--verbose', action='count', default=0, help="Be more communicative. Use -vv for even more information.")
    parser.add_argument('--type_h', action='store', required=True, nargs='*', help="C-Headerfile containing the type information. Multiple references possible.'")
    parser.add_argument('--target_h', action='store', default='input_buffer.h', help="Name of the target C-Headerfile. Default='input_buffer.h'")
    parser.add_argument('--target_cpp', action='store', default='input_buffer.cpp', help="Name of the target C-Sourcefile. Default='input_buffer.cpp'")
    parser.add_argument('--mainapp_path', action='store', required=True, help="path to mainapplication relative to current working directory.")
    parser.add_argument('--ctype_dimension', action='store', type=int, default=1, help="number of buffers to generate (for example for multiple sensor input interfaces).")
    parser.add_argument('buffer_size',action='store', type=int, help="Type to be turned into a buffer.")    
    parser.add_argument('type_name',nargs='*', action = 'store', help="Type to be turned into a buffer.")
    
    args = parser.parse_args(sys.argv[1:])
    
    # post-process arguments
    args.type_name = ' '.join(args.type_name)
    levels = [logging.WARNING, logging.INFO, logging.DEBUG]
    level = levels[min(len(levels)-1,args.verbose)]  # capped to number of levels
    logger.setLevel(level)
    
    # run generation
    generate_files(args.type_name,args.buffer_size, type_filenames=args.type_h, target_header_filename=args.target_h, target_source_filename = args.target_cpp, mainapp_path= args.mainapp_path, ctype_dimension = args.ctype_dimension)