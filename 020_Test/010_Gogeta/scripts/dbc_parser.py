import re
from collections import defaultdict, namedtuple
import signal1

re_message = re.compile(r'BO_ (?P<id>\d+) (?P<name>.*?):.*')
re_signal = re.compile(r'SG_[ \t]+(?P<name>[^ ]+?)[ \t]+(?P<multiplex>[^ ]*)[ \t]*:[ \t]*(?P<startbit>\d+)\|(?P<length>\d+)@(?P<endianness>\d+)(?P<sign>[+-]) \((?P<factor>[0-9.Ee+-]+?),(?P<offset>[0-9.Ee+-]+?)\).*')

def parse_dbc(path_to_dbc):
    with open(path_to_dbc, 'r') as f:
        dbc_lines = [line.strip() for line in f.readlines()]
    _messages = list()
    
    for line in dbc_lines:
        match_message = re_message.match(line)
        if match_message:
            _message = signal1.Message(name = match_message.group('name'), id = match_message.group('id'))
            _messages.append(_message)
        else:
            match_signal = re_signal.match(line)
            if match_signal:
                _signal = signal1.Signal(**match_signal.groupdict(), message = _message)
                multiplex_info = match_signal.group('multiplex')
                if multiplex_info:
                    if multiplex_info == 'M':
                        _message.multiplex_signal = _signal
                    else:
                        _message.multiplex_groups[multiplex_info].append(_signal)
                else:
                    _message.append(_signal)
    return _messages

if __name__ == '__main__':
    import argparse
    import sys
    import generator
    parser = argparse.ArgumentParser(description='Generates C-Types and ILD using dbc information.')    
    
    parser.add_argument('--path_to_dbc', action='store', required=True, help="Path to the dbc file.")
    parser.add_argument('--ctype', action='store', required=True, help="Name of new C_TYPE.")
    parser.add_argument('--output_h', action='store', help='Path to output header.')
    parser.add_argument('--output_ild', action='store', help='Path to output interface link description.')
    
    args = parser.parse_args(sys.argv[1:])
    
    if args.output_h:
        print(generator.Generator.generate_cdeclar(parse_dbc(args.path_to_dbc), args.ctype), file=open(args.output_h,'w'))
    if args.output_ild:
        print(generator.Generator.generate_ild(parse_dbc(args.path_to_dbc), args.ctype), file=open(args.output_ild,'w'))