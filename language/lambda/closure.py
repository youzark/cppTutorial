#!/usr/bin/env python
def namePrinter(name):
    def printName():
        print("My name is " + name)
    printName()

namePrinter("youzark")

def namePrinterFactor(name):
    def printName():
        print("My name is " + name)
    return printName

namePrint = namePrinterFactor("youzark")
namePrint()
