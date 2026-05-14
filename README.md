# Welcome to RedluaOS!

> RedluaOS is an Operating with the goal of being properly documented and also trying to remove the Gatekeeping that the OS development community keeps holding, for some unknown reason.
>

As you can see, none of these goals have been achieved *yet*. Right now i’m only more than 3 Months into this project, so don’t expect much. 

## **IMPORTANT**

- You might notice, that the Project isn't as documented as promised. That is because i'm in kind of a time crisis. I burned the pi 1 B+ so now i have to work with the PI 5. It will take some time to reach the point of a good OS. this isn't that moment.

- the image inside the folder is built for the BCM2712.

- In order to edit the code yourself, you must build a cross-compiler. You're gonna have to search everything up yourself
- I’m currently working on 2 machines. Since i only have a cross compiler on 1, i cannot test the code the other machine. Untested commits are marked with (*UNTESTED*).
- The project is far from done. Keep that in mind. A lot of sudden changes can happen quickly. Don’t expect everything to work flawlessly even if I’m still improving everything over time. I’m trying my best to keep the code as correct and readable as possible.
- If you have suggestions, just put them directly inside ISSUES. I will like any kind of attention i will get. Just a sign that someone sees my project is already a good validation for me.
- But don’t go judging this project too much; I didn’t actually have any bare metal or C experience before, like, at all. I just had the balls to try this out and it somehow worked. But don’t think there wasn’t any pain experienced. There was plenty.


## Project tree:

`/Boot` contains the **Architecture specific boot codes**; they only give enough for the system to jumpt into INIT.

`/Config` contains configuration files (duuhhh) like the linker script, and the config.txt for the raspberry pi firmware

`/Drivers` contains all the **specific Hardware code,**

`/Image` contains the `.img` and `.elf` files. You can flash the img file and/or inspect the elf executable.

`/Kernel` contains all of the Kernel logic code. Contains a Driver API to be able to use the Drivers.

`/Libraries` are well, Libraries. Take a look for yourself at the contents. 

`/Userspace` is a directory for "Programms" that run on top of the entire Ecosystem. Right now, they aren't really... well.. Programms. They kinda act like one.

## Project Structure:

- Folders start with Capital letters, files start with lowercase letters.
- Every `.h` file lives inside a **Global** folder. Everything visible to other TU’s must live inside a header file.
- The Drivers are **Device** oriented. That means that you search after the wished device family, not Architecture. Only Boot is Architecture oriented. This makes for an easier Build system.

## How to build:

Just type:
`make ARCH=*target architecture* DEVICE=*target device family* MODEL=*target Model*`
Replace the value with whatever you target.

**IMPORTANT** 

the values are type sensitive. Make sure you type in the Architecture in Lowercase (example *armv8*), the Device familiy with an Uppercase Initial (example *Broadcom*), and the Model with Uppercase letters only (example *BCM2712*).