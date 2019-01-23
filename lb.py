from tkinter import *


def get_value():
    global theLB
    lbindex = theLB.curselection()
    print(lbindex)
    for x in range(len(lbindex)):
        print(theLB.get(lbindex[x]))


root = Tk()
global theLB
theLB = Listbox(root, selectmode=MULTIPLE)
theLB.pack()
for item in ['aaa', 'bbb', 'ccc', 'ddd', 'eee', 'fff', 'ggg', 'hhh', 'iii']:
    theLB.insert(END, item)
theButton=Button(root, text='GET', command=get_value)
theButton.pack()
root.mainloop()



