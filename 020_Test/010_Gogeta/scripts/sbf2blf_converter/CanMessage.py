import sys
import os
import re
import numpy as np
import struct as st
import can.io.logger

class CanMessage:

    MaxLengthInBit = 64    # Maximum size of the message in bit
    IsExtendedId   = False # Define CAN-ID format

    # Constructor
    def __init__(self, name, CanId):
        self.CanId = CanId
        self.name = name
        self.CurrentLengthInBit = 0            # Current size of the message in bit
        self.signals            = []           # Empty signal list
        self.TimeStamp          = 0.0          # Message time stamp


    def __UpdateDataFrame(self):
        # Reset 64-bit CAN data frame to 0
        self.DataFrame = 0

        # Build frame from signal-tree
        for signal in self.signals:
            self.DataFrame += signal['RawValueCasted'] << signal['StartBit']
            # self.GetDataFrame(True)


    def __CreateBitMask(self, LengthInBit):
        # Init Bit-Mask
        BitMask = 0
        for BitPosition in range(LengthInBit):
            BitMask += 1 << BitPosition
        return BitMask


    def AddSignal(self, name, ScaleFactor, offset, LengthInBit, Startbit):
        SignalLength = self.CurrentLengthInBit + LengthInBit
        # Check for maximum message size
        if  self.MaxLengthInBit >= SignalLength :
            # signal = {'name': 'none' , 'type': 'True:signed / False:unsigned', 'ScaleFactor': 1.0, 'Offset': 0.0, 'LengthInBit': 0, 'StartBit': 0}
            self.signals.append({'name': name , 'ScaleFactor': ScaleFactor, 'offset': offset, 'LengthInBit': LengthInBit, 'StartBit': Startbit, 'BitMask': self.__CreateBitMask(LengthInBit), 'RawValueCasted': 0})
            self.CurrentLengthInBit += LengthInBit
            print('{}: Signal {} added: Current size {}.' .format(self.name, name, self.CurrentLengthInBit))
        else:
            print('{}: Signal {} not added: Maximum message size of {} > {} exceeded!' .format(self.name, name, SignalLength, self.MaxLengthInBit))
            return False
        return True


    def SetSignal(self, name, value):
        # Search for corresponding signal
        for signal in self.signals :
            if signal['name'] == name:
                # Convert signal according to scaling factor and offset
                RawVal = (1.0 / signal['ScaleFactor']) * (value - signal['offset'])

                # Cast raw value to corresponding bit-length
                signal['RawValueCasted'] = signal['BitMask'] & int(RawVal)

                # Build / Rebuild 64 bit message data frame
                self.__UpdateDataFrame()
                return True

        print('{}: Signal {} not found!' .format(self.name, name))

    def GetId(self):
        return self.CanId


    def GetSignals(self):
        return self.signals


    def GetSizeInBit(self):
        return self.CurrentLengthInBit


    def PrintSignals(self):
        print('{} signals:'.format(self.name))
        for signal in self.signals:
            print('Name: {}, ScaleFactor: {}, LengthInBit: {}, StartBit: {}, BitMask: {}' .format(signal['name'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'], np.binary_repr(signal['BitMask'], width=self.MaxLengthInBit) ))


    def PrintSignalLayout(self):
        print('{} signal layout:'.format(self.name))
        for signal in self.signals:
            print('{0:>30}: {1:>64}' .format(signal['name'],  np.binary_repr( (signal['BitMask'] << signal['StartBit'] ), width=self.MaxLengthInBit)))
 

    def GetDataFrame(self, PrintFrame=False):
        if(True == PrintFrame):
            print('{0:>30}: {1:>64}'.format('Data frame', np.binary_repr( self.DataFrame, width=self.MaxLengthInBit)))
        return self.DataFrame

    def GetByteFrame(self, PrintFrame=False):
        self.ByteFrame = st.pack( 'Q', self.DataFrame)
        if(True == PrintFrame):
            print(self.ByteFrame)
        return self.ByteFrame
    

    def SetTimeStamp(self, TimeStamp):
            self.TimeStamp = TimeStamp


    def GetMessage(self):
        return can.Message( arbitration_id=self.CanId,
                            is_extended_id=self.IsExtendedId,
                            timestamp=self.TimeStamp,
                            data=self.GetByteFrame() )
