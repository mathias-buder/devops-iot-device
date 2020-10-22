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
    CanDataFrame = np.uint64(0)

    # Constructor
    def __init__(self, CanId):
        self.CanId = CanId

    def AddSignal(self, name, type, ScaleFactor, offset, LengthInBit, Startbit):

        CurrentLengthInBit = self.CurrentLengthInBit + LengthInBit

        # Check for maximum message size
        if  self.MaxLengthInBit >= CurrentLengthInBit :
            # signal = {'name': 'none' , 'type': 'signed/unsigned', 'ScaleFactor': 1.0, 'Offset': 0.0, 'LengthInBit': 0, 'StartBit': 0}
            self.signals.append({'name': name , 'type': type, 'ScaleFactor': ScaleFactor, 'offset': offset, 'LengthInBit': LengthInBit, 'StartBit': Startbit})
            self.CurrentLengthInBit += LengthInBit
            self.__UpdateDataFrame()
        else:
            print('Signal {} not added: Maximum message size of {} > {} exceeded!' .format(name, CurrentLengthInBit, self.MaxLengthInBit))

    def GetId(self):
        return self.CanId

    def GetSignals(self):
        return self.signals

    def printSignals(self):
        for signal in self.signals:
            print('Name: {}, Type: {}, Scale Factor: {}, LengthInBit: {} StartBit: {}' .format(signal['name'], signal['type'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'] ))

    def __UpdateDataFrame(self):
            self.CanDataFrame = 100
            CanDataFrame = 50



message1 = CanMessage(0x1)



message1.AddSignal('signal_1', 'unsigned', 1.0, 0.0, 10, 2)
message1.AddSignal('signal_2', 'unsigned', 2.0, 1.0, 54, 5)
message1.AddSignal('signal_3', 'unsigned', 2.0, 1.0, 1, 5)

for signal in message1.signals:
    print('Name: {} {} {} {} {}' .format(signal['name'], signal['type'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'] ))
