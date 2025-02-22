
""" 
sudo cp sayHello.service /usr/lib/systemd/   
or this:   
sudo cp sayHello.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable sayHello.service 
sudo systemctl start sayHello.service 
sudo systemctl status sayHello.service
to track: htop -> /startupApp
"""

import tkinter as tk

wd=tk.Tk()
wd.geometry("300x200+100+100")
lbl=tk.Label(wd,text="Hello Ehab")
lbl.pack()
wd.mainloop()