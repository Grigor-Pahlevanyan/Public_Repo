import os

cmd = 'notepad'
os.chdir("C:\\Users\\t0268052\\Desktop\\SAP\\Quick Login")
flags = os.O_RDWR | os.O_CREAT
sap_file = os.open("tx.sap",)
#os.system(cmd)