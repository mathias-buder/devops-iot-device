import sys
import os
import re
import numpy as np
import can.io.logger

class CanMessage:

    # Global member variables
    CanId = 0               # Default CAN-ID
    MaxLengthInBit = 64     # Maximum size of the message in bit
    CurrentLengthInBit = 0  # Current size of the message in bit
    signals = []            # Empty signal list
    CanDataFrame_u64 = np.uint64(0)

    # Constructor
    def __init__(self, CanId):
        self.CanId = CanId

    def AddSignal(self, name, type, ScaleFactor, offset, LengthInBit, Startbit):
        SignalLength = self.CurrentLengthInBit + LengthInBit
        # Check for maximum message size
        if  self.MaxLengthInBit >= SignalLength :
            # signal = {'name': 'none' , 'type': 'signed/unsigned', 'ScaleFactor': 1.0, 'Offset': 0.0, 'LengthInBit': 0, 'StartBit': 0}
            self.signals.append({'name': name , 'type': type, 'ScaleFactor': ScaleFactor, 'offset': offset, 'LengthInBit': LengthInBit, 'StartBit': Startbit, 'BitMask': self.__CreateBitMask(LengthInBit)})
            self.CurrentLengthInBit += LengthInBit
            print('Signal {} added: Current size {}.' .format(name, self.CurrentLengthInBit))
        else:
            print('Signal {} not added: Maximum message size of {} > {} exceeded!' .format(name, SignalLength, self.MaxLengthInBit))
            return False
        return True

    def SetSignal(self, name, value):
        # Search for corresponding signal
        for signal in self.signals:
            if signal['name'] == name:
                # Convert signal according to scaling factor and offset
                RawVal = (1.0 / signal['ScaleFactor']) * (value - signal['offset'])

                # Cast raw value to corresponding signal bit-length
                break

    def GetId(self):
        return self.CanId

    def GetSignals(self):
        return self.signals

    def GetSizeInBit(self):
        return self.CurrentLengthInBit

    def PrintSignals(self):
        print('Signals:')
        for signal in self.signals:
            print('Name: {}, Type: {}, ScaleFactor: {}, LengthInBit: {}, StartBit: {}, BitMask: {}' .format(signal['name'], signal['type'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'], np.binary_repr(signal['BitMask'], width=self.MaxLengthInBit) ))

    def __UpdateDataFrame(self):
            self.CanDataFrame_u64 = 100
    
    def __CreateBitMask(self, LengthInBit):
        # Init Bit-Mask
        BitMask = 0
        for BitPos in range(LengthInBit):
            BitMask |= 1<<BitPos
        return BitMask





message1 = CanMessage(0x1)



message1.AddSignal('signal_1', 'unsigned', 0.1, 0.0, 10, 2)
message1.AddSignal('signal_2', 'unsigned', 2.0, 1.0, 54, 5)
message1.AddSignal('signal_3', 'unsigned', 2.0, 1.0, 1, 5)

message1.SetSignal('signal_1', 10)

message1.PrintSignals()


#for signal in message1.signals:
#    print('Name: {}, Type: {}, ScaleFactor: {}, LengthInBit: {}, StartBit: {}, BitMask: {}' .format(signal['name'], signal['type'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'], np.binary_repr(signal['BitMask'], width=64) ))