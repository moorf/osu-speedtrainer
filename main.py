import time
import keyboard

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
late = 0
punishmentms = 30

b1 = 'r'
b2 = 't'
bpm = 170

def main():
    global fail, hook, time_window, bpm
    
    #bpm = get_bpm()
    time_window = (60 / (bpm * 4)) * 1000
    print(f"Press the buttons alternately at {bpm} BPM. You have {time_window:.2f} ms to press the next button. Buttons: {b1} {b2}")
    hook = keyboard.on_press(check_for_keys)
    start_time = time.time()

    while fail<1:
        time.sleep(0)
            
        

def check_for_keys(e):
    global clicks, alternate, alternate2, fail, start, time_window, start_time, error, b1,b2, late
    if e.event_type == keyboard.KEY_DOWN:
    
        if start == 0:
            start_time = time.time()
            start = 1
            
        if e.name == b1:
            elapsed_time = time.time() - start_time
            milliseconds = int((elapsed_time % 1) * 1000)
            #print(milliseconds, time_window)
            print(milliseconds)
            if milliseconds>time_window:
                error+=1
                late = milliseconds - time_window
            else:
                error=0
                late = 0
            if error == 3 or late>punishmentms:
                print(f"Too late. Clicks: {clicks}")
                fail=1
            
            start_time = time.time()
            clicks += 1
            alternate += 1
            alternate2 = 0
            if alternate > 1:
                print(f"Alternate error. Clicks: {clicks}")
                fail=1
            
        elif e.name == b2:
            elapsed_time = time.time() - start_time
            milliseconds = int((elapsed_time % 1) * 1000)
            #print(milliseconds, time_window)
            print(milliseconds)
            if milliseconds>time_window:
                error+=1
                late = milliseconds - time_window
            else:
                error=0
                late = 0
            if error == 3 or late>punishmentms:
                print(f"Too late. Clicks: {clicks}")
                fail=1  
            start_time = time.time()
            clicks += 1
            alternate2 += 1
            alternate = 0
            if alternate2 > 1:
                print(f"Alternate error. Clicks: {clicks}")
                fail=1
                

def get_bpm():
    try:
        bpm = int(input("Enter the BPM (beats per minute): "))
        if bpm <= 0:
            print("Please enter a positive BPM value.")
        else:
            return bpm
    except ValueError:
        print("Invalid input. Please enter a valid integer.")

def cleanup():
    global hook
    keyboard.unhook_all()

if __name__ == "__main__":
    try:
        main()
    finally:
        cleanup()
        
