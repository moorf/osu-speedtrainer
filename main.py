import time
import keyboard
import os

clicks = 0
alternate = 0
alternate2 = 0
fail = 0    
start = 0
time_window = 1000
start_time = 0
error = 0
b1 = 'r'
b2 = 't'
bpm = 180
late = 0
restart = True
milliseconds = 0

os.system('color 3')

punishmentms = 30

config_filename = "config.cfg"

quitbutton = 'q'
restartbutton = '`'
bpmbutton = 'p'


    
def main():
    global fail, hook, time_window, bpm, restart,start, clicks, alternate, alternate2, start_time, error,b1,b2,punishmentms
    
    read_config(config_filename)
    
    time_window = (60 / (bpm * 4)) * 1000
    
    start_time = time.time()
    while start<2:
        if restart:
            os.system('clear')
            print(f"Press the buttons alternately at {bpm} BPM. You have {time_window:.2f} ms to press the next button. Buttons: {b1} {b2}.")
                
            clicks = 0
            alternate = 0
            alternate2 = 0
            fail = 0    
            start = 0
            start_time = 0
            error = 0
            
            hook = keyboard.on_press(check_for_keys)
            restart = False
            
            while fail<1:
                time.sleep(0)
            
def restarter(e):       
    global clicks, alternate, alternate2, fail, start, time_window, start_time, error, b1,b2, late, restart, hook, quitbutton, restartbutton
    if e.event_type == keyboard.KEY_DOWN: 
        if e.name == quitbutton:
            start+=1
            
        if e.name == restartbutton:
            restart = True
        
        if e.name == bpmbutton:
            bpmchange()     
            
def check_for_keys(e):
    global clicks, alternate, alternate2, fail, start, time_window, start_time, error, b1,b2, late, restart, hook, quitbutton, restartbutton
    
    if e.event_type == keyboard.KEY_DOWN:
    
        if start == 0 and e.name == b1 or start == 0 and e.name == b2:
            start_time = time.time()
            start = 1
              
        if e.name == b1:
            erroroutput()

            alternate += 1
            alternate2 = 0
            if alternate > 1:
                alternateerror()
                
            
        elif e.name == b2:
            erroroutput()    
            
            alternate2 += 1
            alternate = 0
            if alternate2 > 1:
                alternateerror()

def alternateerror():
    global clicks, alternate, alternate2, fail, start, time_window, start_time, error, b1,b2, late, restart, hook, quitbutton, restartbutton, milliseconds
    print(f"Alternate error. Clicks: {clicks}.       {quitbutton} to quit,  {restartbutton} to restart.  {bpmbutton} to change bpm.")
    fail=1
    keyboard.unhook_all()
    keyboard.on_press(restarter) 
                
def erroroutput():
    global clicks, alternate, alternate2, fail, start, time_window, start_time, error, b1,b2, late, restart, hook, quitbutton, restartbutton, milliseconds
    elapsed_time = time.time() - start_time
    milliseconds = int((elapsed_time % 1) * 1000)
    if milliseconds>time_window:
        error+=1
        late = milliseconds - time_window
        print(f"-----   {milliseconds}")
                
    else:
        error=0
        late = 0
        print(milliseconds)
    if error == 3 or late>punishmentms:
        print(f"Too late. Clicks: {clicks}.      {quitbutton} to quit,  {restartbutton} to restart.  {bpmbutton} to change bpm.")
        fail=1
        keyboard.unhook_all()
        keyboard.on_press(restarter) 
    start_time = time.time()
    clicks += 1    
               

def bpmchange():
    global bpm, time_window, config_filename
    bpm = get_bpm()
    time_window = (60 / (bpm * 4)) * 1000
    with open(config_filename, 'r') as file:
        lines = file.readlines()
        lines[2] = str(bpm) + '\n'
    with open(config_filename, 'w') as file:
        file.writelines(lines)
    print(f"Saved. BPM is {bpm}. {quitbutton} to quit,  {restartbutton} to restart.  {bpmbutton} to change bpm.")
    
    
def get_bpm():
    try:
        bpm = int(input("Enter the BPM (beats per minute): "))
        
        
        if bpm <= 0:
            print("Please enter a positive BPM value.")
        else:
            return bpm
    except ValueError:
        print("Invalid input. Please enter a valid integer.")

def create_default_config(filename):
    with open(filename, 'w') as f:
        f.write("e\n")
        f.write("r\n")
        f.write("180\n")
        f.write("30\n")
        f.write("q\n")
        f.write("`\n")
        f.write("p\n")

def read_config(filename):
    global b1, b2, bpm, punishmentms, quitbutton, restartbutton, bpmbutton
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
            b1 = lines[0].strip()
            b2 = lines[1].strip()
            bpm = int(lines[2].strip())
            punishmentms = int(lines[3].strip())
            quitbutton = lines[4].strip()
            restartbutton = lines[5].strip()
            bpmbutton = lines[6].strip()
    except FileNotFoundError:
        print(f"Config file '{filename}' not found. Creating a new one with default values.")
        create_default_config(filename)

def cleanup():
    global hook
    keyboard.unhook_all()

if __name__ == "__main__":
    try:
        main()
    finally:
        cleanup()
        
