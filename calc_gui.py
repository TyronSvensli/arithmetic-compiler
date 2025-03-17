import os
import subprocess
import tkinter as tk

def button_click(value):
    current = entry.get()
    entry.delete(0, tk.END)
    entry.insert(0, current + value)

def clear_entry():
    entry.delete(0, tk.END)

def compile_result():
    #write expression to file
    fd = os.open("generated/1_expression.txt", os.O_WRONLY | os.O_CREAT)
    file = os.fdopen(fd, 'w')
    file.write(entry.get())
    file.close()
    exit()

root = tk.Tk()
root.title("calc (short for calculator)")
entry = tk.Entry(root, width=30, font=("Arial", 16), justify="right")
entry.grid(row=0, column=0, columnspan=4)

buttons = [
    ('7', 1, 0), ('8', 1, 1), ('9', 1, 2), ('/', 1, 3),
    ('4', 2, 0), ('5', 2, 1), ('6', 2, 2), ('*', 2, 3),
    ('1', 3, 0), ('2', 3, 1), ('3', 3, 2), ('-', 3, 3),
    ('C', 4, 0), ('0', 4, 1), ('=', 5, 3), ('+', 4, 3),
    ('(', 4, 2), (')', 5, 2),
]

for (text, row, col) in buttons:
    if text == "=":
        action = compile_result
    elif text == "C":
        action = clear_entry
    else:
        action = lambda val=text: button_click(val)
    tk.Button(root, text=text, width=7, height=2, command=action).grid(row=row, column=col)

root.mainloop()