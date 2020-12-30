# -*- coding: utf-8 -*-

import re
import argparse
import sys
import xml.etree.ElementTree as ET
import functools as ft
import operator as op
import itertools as it
import os.path
from collections import defaultdict

class XMLCombiner(object):
    """ props to http://stackoverflow.com/questions/14878706/merge-xml-files-with-nested-elements-without-external-libraries"""
    def __init__(self, filenames):
        assert len(filenames) > 0, 'No filenames!'
        # save all the roots, in order, to be processed later
        self.roots = [ET.parse(f).getroot() for f in filenames]

    def combine(self):
        for r in self.roots[1:]:
            # combine each element with the first one, and update that
            self.combine_element(self.roots[0], r)
        # return the string representation
        return ET.ElementTree(self.roots[0])

    def combine_element(self, one, other,path=""):
        """
        This function recursively updates either the text or the children
        of an element if another element is found in `one`, or adds it
        from `other` if not found.
        """
        # Create a mapping from tag name to element, as that's what we are fltering with
        
        
        mapping = {(el.tag, el.attrib.get('name',''), el.attrib.get('cond','')): el for el in one}
        for el in other:
            name = el.attrib.get('name','')
            cond = el.attrib.get('cond','')
            if el.tag in {'externs','selection-models'}:
                one.append(el)
            elif len(el) == 0:
                # Not nested
                try:
                    # Update the text
                    mapping[(el.tag, name, cond)].text = el.text
                    mapping[(el.tag, name, cond)].attrib.update(el.attrib)
                    print('Warning: multiple declarations of variable ',path+name, ", cond = ", cond)
                except KeyError:
                    # An element with this name is not in the mapping
                    mapping[(el.tag, name, cond)] = el
                    # Add it
                    one.append(el)
            else:
                try:
                    # Recursively process the element, and update it in the same way
                    self.combine_element(mapping[(el.tag, name, cond)], el, path+name+'.')
                except KeyError:
                    # Not in the mapping
                    mapping[(el.tag, name, cond)] = el
                    # Just add it
                    one.append(el)


class Signal:
    struct_names = defaultdict(int)

    def custom_vars(self,indent=4):
        if self.cond:
            c_start = "#if ({})\n".format(self.cond)
            c_end = "#endif /*{}*/\n".format(self.cond)
        else:
            c_start, c_end = ("","")
        
        array_string = "".join('[{}]'.format(a) for a in self.array)
        this_name = self.name.replace('[','_').replace(']','_')
        if self.datatype == "struct":
            struct_name = this_name.upper()
            
            new_struct_name = struct_name
            if self.struct_names[struct_name] != 0:
                new_struct_name += str(self.struct_names[struct_name])
                
            self.struct_names[struct_name] += 1
            
            child_string = ""
            for child in self.children:
                child_string+=child.custom_vars(indent+4)
            if child_string:
                result = indent*" "+"struct {}_TAG\n".format(new_struct_name)
                result+= indent*" "+"{\n"
                result += child_string
                result+= indent*" "+"} "+this_name+array_string+";\n"
                return c_start+result+c_end
            else:
                return ""
        else:
            return c_start+indent*" "+self.c_datatype()+" "+this_name+array_string+";\n"+c_end

    def __init__(self, node, parent_is_custom_var=False):
        """ parse signals from xml node
        """
        properties = node.attrib
        self.name = properties.get('name','')
        self.pointertype = properties.get('type','')
        self.datatype = node.tag
        self.cond = properties.get('cond','')
        self.alias = properties.get('alias',self.name)
        self.expr = properties.get('expr','{value}')
        self.pointer = properties.get('pointer','False').lower()=='true'
        self.internal = properties.get('internal','false')
        self.format = properties.get('format','DECIMAL')
        self.tpt_parameter=properties.get('tpt-parameter','false').lower()=='true'
        self.unit = properties.get('unit','').encode('ascii', 'ignore').decode("utf-8")
        self.decimals = properties.get('decimals',"3" if self.datatype in {'float','double'} else "0")
        self.array = tuple(x for x in properties.get('array','').split(',') if x)
        self.custom_var = parent_is_custom_var or self.pointer or self.expr!='{value}'

        self.ranges = ['<range-name lower-bound="{lower}" text="{text}" upper-bound="{upper}"/>'.format(
                text=child.text,
                lower=float(child.attrib.get('value', child.attrib.get('min','-Infinity'))),
                upper=float(child.attrib['value']) + 10e-4 if 'value' in child.attrib else child.attrib.get('max','Infinity'))
            for child in node if child.tag == "range"]

        self.defaultvalue = properties.get("default","({datatype})0".format(datatype=self.c_datatype()))
        if node.tag in {'struct', 'properties'}:
            self.primitive = False
            self.children = [Signal(child, self.custom_var) for child in node if child.tag in {'double','float','uint8','uint16','uint32','int8','int16','int32','boolean','enum','struct'} and child.attrib.get('active',"True").lower()=="true"]
        else:
            self.children=[]
            self.primitive = True

    def __iter__(self):
        return iter(self.children)

    def __hash__(self):
        return hash(self.name)

    def __repr__(self):
        return self.name

    def c_datatype(self):
        if self.datatype == "enum":
            if self.pointertype:
                return self.pointertype
            else:
                return 'uint32'
        elif self.datatype.startswith('int'):
            return 's'+self.datatype
        else:
            return self.datatype

    type_to_wrapper={'double':'F64','float':'F32','uint8':'U8','uint16':'U16','uint32':'U32','int8':'S8','int16':'S16','int32':'S32','boolean':'BOOLEAN','enum':'U32'}

    def get_copy_to_shared(self, path = "", array_index = "", prefix = '    ',j=0,default=False, full_path=None):
        if full_path == None:
            full_path = args.wrapperstruct+"."
        def full_qualified_name():
            return path + self.name + array_index
        def get_new_path():
            if self.pointertype:
                return self.alias + '->'
            if self.pointer:
                return full_qualified_name() + '->'
            if self.datatype == 'struct':
                return full_qualified_name() + '.'
            return path # root node

        def get_full_path():
            if self.datatype == 'struct':
                return full_path + self.name.replace('[','_').replace(']','_') + array_index + '.'
            return full_path + self.name.replace('[','_').replace(']','_') + array_index
        
        if self.cond:
            c_start = "#if ({})\n".format(self.cond)
            c_end = "#endif /*{}*/\n".format(self.cond)
        else:
            c_start, c_end = ("","")
        
        if self.array and not array_index:
            result = ""
            indices = self.array
            for i,M in enumerate(indices):
                result += "{prefix}for (int i{i}=0; i{i}<{M}; ++i{i})\n{prefix}{{\n".format(i=i+j,M=M,prefix=prefix)
                prefix += '    '
            result += self.get_copy_to_shared( path, ''.join('[i{i}]'.format(i=j+i) for i in range(len(indices))), prefix, j+len(indices),default,full_path)
            for i,M in enumerate(self.array):
                prefix = prefix[:-4]
                result += prefix+"}\n"
            return c_start+result+'\n'+c_end
        else:
            if self.datatype in {'double','float','uint8','uint16','uint32','int8','int16','int32','boolean','enum'}:
                expr = self.expr.format(value=full_qualified_name())
                if self.pointer and not default:
                    expr = '((NULL!={name})?*({expr}):{defaultvalue})'.format(name=full_qualified_name(), expr=expr, defaultvalue=self.defaultvalue)
                elif default:
                    expr = '{defaultvalue}'.format(defaultvalue=self.defaultvalue)

                if self.datatype == 'enum' and not self.pointertype:
                    expr = '(uint32)'+expr;

                result = prefix+"{newname}={expr:s};\n".format(expr=expr,newname=get_full_path())
                return c_start+result+c_end
            else:
                if self.expr!="{value}" and not self.pointertype:
                    raise ValueError("Struct Pointer {} needs 'type'".format(full_qualified_name()))
                if self.pointertype and not default:
                    ref = '&' if not self.pointer else ''
                    result = "{prefix}{pointertype}* {alias} = {ref}{expr};\n".format(prefix=prefix,alias=self.alias,ref=ref, expr=self.expr.format(value=path+self.name+array_index),pointertype=self.pointertype)
                else:
                    result = ""

                newprefix = prefix + ('        ' if self.pointer and not default else '')
                normal_assignments = ''.join(child.get_copy_to_shared(get_new_path(), prefix=newprefix , j=j, full_path = get_full_path()) for child in self) if not default else ''
                default_assignments = ''.join(child.get_copy_to_shared(get_new_path(), prefix=newprefix, j=j, default=True, full_path = get_full_path()) for child in self) if self.pointer or default else ''
                alias = self.alias if self.pointertype else path+self.name+array_index

                if default:
                    return c_start+result+default_assignments+c_end
                elif self.pointer:
                    return c_start+'\n'+prefix+'{\n    '+result+"""{prefix}    if (NULL!={alias})\n{prefix}    {{
{normal_assignments}{prefix}    }} else\n{prefix}    {{
{default_assignments}{prefix}    }}
""".format(prefix=prefix,alias=alias,normal_assignments=normal_assignments,default_assignments=default_assignments)+prefix+'}\n'+c_end
                else:
                    return c_start+result+normal_assignments+c_end

def get_externals(node, prefix=''):
    externals = []
    
    for externs in node:
        if externs.tag=='externs':
            for extern in externs:
                if extern.tag=='extern':
                    if not extern.text.strip().startswith('#'):
                        semicolon = ';'
                    else:
                        semicolon = ''
                    cond = extern.attrib.get("cond",None)
                    if cond:
                        externals.append("#if ({})".format(cond))
                        additional_indent = "    "
                    else:
                        additional_indent = ""
                    
                    externals.append(additional_indent + extern.text+semicolon)
                    if cond:
                        externals.append("#endif /*{}*/".format(cond))
    
    return prefix + ('\n'+prefix).join(e for e in externals)+'\n'

def wrapper_data(signal):
    return """typedef struct WRAPPER_DATA_TAG
{{
{customvars}
}} WRAPPER_DATA;
WRAPPER_DATA {wrapperstruct};
""".format(customvars="".join(s.custom_vars() for s in signal.children),wrapperstruct=args.wrapperstruct)

def generate_wrapper_c(out_filename, xml_data, header_filename, structname):
    s = Signal(xml_data)
    externals = get_externals(xml_data,prefix='')
    copy_code = s.get_copy_to_shared()

    content = """#include <string.h>\n#include"{header_filename}"

/* external declarations and includes from declarations.xml */
{externals}

/* wrapper datastructure */
{wrapper_data}

void copy_{structname}(void)
{{
{copy_code}
}}

void init_{structname}(void)
{{
    (void)memset(&{wrapperstruct},0,sizeof(WRAPPER_DATA));
}}

""".format(header_filename=header_filename, externals=externals, wrapper_data=wrapper_data(s), structname=structname, copy_code=copy_code, wrapperstruct=args.wrapperstruct)

    print(content, file = open(out_filename, 'w'))


def generate_wrapper_h(out_filename, structname):
    content = """#ifndef {guard}
#define {guard}

void copy_{structname}(void);
void init_{structname}(void);

#endif /* {guard} */
""".format(guard=out_filename.rsplit('/',1)[-1].upper().replace('.','_'), structname=structname)

    print(content, file = open(out_filename, 'w'))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate Project.esx and Wrapper')
    parser.add_argument('-x','--xml',action = "store",nargs='+',default=[])
    parser.add_argument('-wout','--wrapper_out',action = "store",default=None)
    parser.add_argument('-hout','--header_out',action = "store",default=None)
    parser.add_argument('-ws','--wrapperstruct',action="store", default="_wrapper_s")

    args = parser.parse_args(sys.argv[1:])

    if args.xml:
        xml_data = XMLCombiner(args.xml).combine().getroot()
    else:
        xml_data = ET.Element("properties")

    if args.wrapper_out:
        print("Generating", args.wrapper_out)
        generate_wrapper_c(args.wrapper_out, xml_data, args.header_out, args.wrapperstruct)
    if args.header_out:
        print("Generating", args.header_out)
        generate_wrapper_h(args.header_out, args.wrapperstruct)