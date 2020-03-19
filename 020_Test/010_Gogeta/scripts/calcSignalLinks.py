import dbc_parser
import re
from enum import Enum

re_properties_line = re.compile(r'(?P<dbc_parent_name>[\w]+)/(?P<dbc_child_name1>[\w]+)(?P<dbc_arrayindex>\$?)(?P<dbc_child_name2>[\w]*) *= *(?P<c_parent_name>[\w\[\]\.]+)(?P<c_arrayindex>\$?)\]?\.(?P<c_child_name>[\w\[\]\.]*)')

def getDBCMessages(filePath,fileName):
    path_to_dbc = filePath + "../dbc/" + fileName + ".dbc"
    return dbc_parser.parse_dbc(path_to_dbc)

def getArraySizeFromDBCFile(parent_name, child_name_prefix, child_name_postfix, messages):
    arraySize = 0
    
    re_extract_digit = re.compile(child_name_prefix + '(?P<digit>[\\d]+)' + child_name_postfix + '.*')
    
    for message in messages:
        if message.name != parent_name:
            continue
        
        for signals in message.multiplex_groups.values():
            for signal_ in signals:
                match = re_extract_digit.match(signal_.name)
                if match:
                    digit = int(match.group('digit'))
                    if digit > arraySize:
                        arraySize = digit
                    
    return arraySize

def getMultiplexID(dbc_messages, message_name, signal_name):
    
    for message in dbc_messages:
        if message.name != message_name:
            continue
        
        #for normal_signal in message:
        #    if normal_signal.name == signal_name:
        #        return None

        for mux_key, mux_signals in message.multiplex_groups.items():
            for signal in mux_signals:
                if signal.name == signal_name:
                    return int(mux_key[1:])
    
    return None

def appendSignal(newContent, dbc_messages, dbctype, blfLoaderPrefix, dbc_message_name, dbc_signal_name, dllLibString):
    burst_addition = ''
    if dbctype == DBCType.BURST:
        burst_addition = 'burst_vs[0].'
        
    transfer_all_addition =''
    if dbctype == DBCType.TRANSFER_ALL:
        transfer_all_addition = 'burst_vs[0].'
        
    multiplexID = getMultiplexID(dbc_messages,dbc_message_name,dbc_signal_name)
    if(multiplexID != None):
        dbc_message_name += '_' + str(multiplexID)
        
    newContent.append('  \"' + blfLoaderPrefix + '.' +  burst_addition + dbc_message_name + '.' + transfer_all_addition + dbc_signal_name + ' -> ' + dllLibString + "\"")
    

class DBCType(Enum):
    MASTER = 1
    BURST = 2
    TRANSFER_ALL = 3

def calcSignalLinksForFile(filePath,fileName,dllLibName,dbcMessageName, dbctype):
    blf_loader_name = 'BLFLoader'
    dbc_messages = getDBCMessages(filePath,dbcMessageName)

    file = open(filePath + fileName +".properties","r")
    
    fileName = fileName.replace('.','_')
    dbcMessageName = dbcMessageName.replace('.','_')
    
    newContent = list()
    #dbcNameTupleToArraySize = dict()
    
    blfLoaderPrefix = blf_loader_name + '.' + dbcMessageName
    for line in file:
        if line[0] == "#":
            continue
        
        match = re_properties_line.match(line)
        if match:
            if match.group('dbc_arrayindex'):
                dbc_message_name = match.group('dbc_parent_name')
                dbc_child_name1 = match.group('dbc_child_name1')
                dbc_child_name2 = match.group('dbc_child_name2')
                
                #arraySize = dbcNameTupleToArraySize.get((dbc_parent_name,dbc_child_name1,dbc_child_name2))
                #if not arraySize:
                arraySize = getArraySizeFromDBCFile(dbc_message_name, dbc_child_name1,dbc_child_name2,dbc_messages)
                   # dbcNameTupleToArraySize[(dbc_parent_name,dbc_child_name1,dbc_child_name2)] = arraySize
                
                for i in range(arraySize):
                    blfLoaderPrefix = blf_loader_name + '.' + dbcMessageName
                    dbc_signal_name = dbc_child_name1 + str(i) + dbc_child_name2
                    dllLibString = dllLibName + '.' + match.group('c_parent_name') + str(i) + '].' + match.group('c_child_name')
                    
                    appendSignal(newContent, dbc_messages, dbctype, blfLoaderPrefix, dbc_message_name, dbc_signal_name, dllLibString)    
                    
            else:
                dbc_message_name = match.group('dbc_parent_name')
                dbc_signal_name = match.group('dbc_child_name1')
                dllLibString = dllLibName + '.' + match.group('c_parent_name') + '.' + match.group('c_child_name')
                
                appendSignal(newContent, dbc_messages, dbctype, blfLoaderPrefix, dbc_message_name, dbc_signal_name, dllLibString)
        
        #newContent.append(line)
    return newContent

def calcSignalLinks():
    dllLibName = "libWeGetASim"
    filePath = "../../ACSim/Config/Development/GUI_Config/can/properties/"
    newContent = calcSignalLinksForFile(filePath,"A087MB_V3.4_AC1kT",dllLibName,"A087MB_V3.4_AC1kT",DBCType.MASTER)
    newContent.extend(calcSignalLinksForFile(filePath,"Radar_Input_Output",dllLibName,"Radar_Input_Output", DBCType.TRANSFER_ALL))
    newContent.extend(calcSignalLinksForFile(filePath,"ZF_ISZ_VC65_Fusion_Protocol_SCAM4_B4",dllLibName,"ZF_ISZ_VC65_Fusion_Protocol_SCAM4_B4",DBCType.BURST))
    #messages = getDBCMessages(filePath,"A087MB_V3.4_Isuzu")
    #print(getArraySizeFromDBCFile("DL_Tracks","tr",messages))
    
    newfile = open("Links.json","w")
    newfile.write(",\n".join(newContent) + "\n")
    #print(newContent)


calcSignalLinks()