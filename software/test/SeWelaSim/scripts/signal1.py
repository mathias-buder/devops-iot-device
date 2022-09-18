from decimal import Decimal
from ctypes.test.test_pickling import name
from collections import defaultdict
import re

def c_float_literal(s):
    if '.' not in s and 'E' not in s.upper():
        s = s+'.'
    return s+'F'

class Message(list):
    def __init__(self, name, id):
        super().__init__(self)
        self.name = name
        self.id = id
        self.multiplex_signal = None
        self.multiplex_groups = defaultdict(list)
        self.duplicate = False
        self.indices = dict()
        
    def __eq__(self, other):
        # ignore name and id
        if len(self) != len(other):
            return False
        for x in self:
            for y in other:
                if x == y:
                    break
            else:
                return False
        
        if self.multiplex_signal != other.multiplex_signal:
            return False
        
        for mpg in sorted(self.multiplex_groups.keys()):
            if not other.multiplex_groups[mpg]:
                return False
            if len(self.multiplex_groups[mpg]) != len(other.multiplex_groups[mpg]):
                return False
            for x in self.multiplex_groups[mpg]:
                for y in other.multiplex_groups[mpg]:
                    if x == y:
                        break
                else:
                    return False
        return True

class Signal:
    properties = None
    
    def __eq__(self, other):
        return self.multiplex == other.multiplex and \
                self.startbit == other.startbit and \
                self.endianness == other.endianness and \
                self.sign == other.sign and \
                self.length == other.length and \
                self.factor == other.factor and \
                self.offset == other.offset and \
                self.ild == other.ild # instead of name
    
    @property
    def fullname(self):
        return self.message.name+'/'+self.name
    
    @property
    def ild(self):
        if not self._property:
            for p in self.__class__.properties:
                m =  p.signal_matcher.match(self.fullname)
                if m:
                    self.message.indices.update(m.groupdict())
                    self._property = p
                    break
        return self._property
    
    def var(self):
        p = self.ild
        
        if p:
            m =  p.signal_matcher.match(self.fullname)
            if m:
                rep = {re.escape('${{{k}}}'.format(k=k)):k+'_u16' for k,v in m.groupdict().items()}
                if rep:
                    pattern = re.compile("|".join(rep.keys()))    
                    result =  pattern.sub(lambda m: rep[re.escape(m.group(0))], p.target_pattern)
                    result = re.sub(r'\[0+(\d+)\]',r'[\1]',result)
                else:
                    result =  p.target_pattern
                return 'p_result_s->'+result
        else:
            raise ValueError()
    
    def __init__(self, *args, **kwargs):
        self.__dict__.update(kwargs)
        self._property = None
    
    @property
    def logical_c_suffix(self):
        suffix = '_'+self.logical_ctype.lower()
        if suffix == '_boolean':
            suffix = '_b'
        return suffix
    
    @property
    def raw_c_suffix(self):
        suffix = '_'+self.raw_ctype.lower()
        if suffix == '_boolean':
            suffix = '_b'
        return suffix
    
    @property
    def c_name(self):
        suffix = self.logical_c_suffix
        if self.name.endswith(suffix):
            return self.name
        else:
            return self.name+suffix
    
    @property
    def c_offset(self):
        if self.is_float():
            return c_float_literal(self.offset)
        return self.offset
    
    @property
    def c_factor(self):
        if self.is_float():
            return c_float_literal(self.factor)
        return self.factor
    
    @property
    def c_inv_factor(self):
        inv_factor = '{:f}'.format(1/Decimal(self.factor))
        if self.is_float():
            return c_float_literal(inv_factor)
        return inv_factor
    
    @property
    def c_max_value(self):
        if self.is_float():
            return c_float_literal(str(self.max_value))
        return str(self.max_value)
    
    @property
    def c_min_value(self):
        if self.is_float():
            return c_float_literal(str(self.min_value))
        return str(self.min_value)    
 
    @property
    def min_value(self):
       if self.is_float():
           offset = Decimal(self.offset)
           factor = Decimal(self.factor)
       else:
           offset = int(self.offset)
           factor = int(self.factor)
           
       if self.sign == '+':
           return offset
       else:
           return -factor * 2**(int(self.length)-1)+offset
    
    @property
    def rte_endianness(self):
        if self.endianness == '0':
            return 'COM_BIG_ENDIAN'
        else:
            return 'COM_LITTLE_ENDIAN'
    
    @property
    def rte_sign(self):
        if self.sign=='-':
            return 'COM_SINT64'
        else:
            return 'COM_UINT64'
    
    @property
    def max_value(self):
       if self.is_float():
           offset = Decimal(self.offset)
           factor = Decimal(self.factor)
       else:
           offset = int(self.offset)
           factor = int(self.factor)
           
       if self.sign == '+':
           return offset + 2**(int(self.length)) - 1
       else:
           return factor * (2**(int(self.length)-1)-1)+offset        
    
    @property
    def raw_ctype(self):
       length = int(self.length)
       if self.sign == '+':
           if length <= 1:
               return 'BOOLEAN'
           elif length <= 8:
               return 'U8'
           elif length <= 16:
               return 'U16'
           elif length <=32:
               return 'U32'
           else:
               return 'U64'
       else:
           if length <= 8:
               return 'S8'
           elif length <= 16:
               return 'S16'
           elif length <= 32:
               return 'S32'
           else:
               return 'S64'

    @property
    def logical_ctype(self):
        if self.length == '1' and self.factor=='1' and self.sign == '+' and self.offset == '0':
            return 'BOOLEAN'
        elif '.' in self.factor or '.' in self.offset:
            return 'F32'
        elif self.sign == '+':
            max = int(self.factor)*(2**int(self.length)-1)+int(self.offset)
            if max<2**8:
                return 'U8'
            elif max<2**16:
                return 'U16'
            elif max<2**32:
                return 'U32'
            elif max<2**64:
                return 'U64'
            else:
                return 'F32'
        else:
            max = int(self.factor) * (2**(int(self.length)-1)-1)+int(self.offset)
            min = -int(self.factor) * 2**(int(self.length)-1)+int(self.offset)
            if max<2**7 and min>=-2**7:
                return 'S8'
            elif max<2**15 and min>=-2**15:
                return 'S16'
            elif max<2**31 and min>=-2**31:
                return 'S32'
            elif max<2**63 and min>=-2**63:
                return 'S64'
            else:
                return 'F32'
            
    def is_float(self):
        return '.' in self.factor or '.' in self.offset or 'E' in self.factor.upper() or 'E' in self.offset.upper()