# Welcome to RedluaOS!

> RedluaOS is an Operating with the goal of being properly documented and also trying to remove the Gatekeeping that the OS development community keeps holding, for some unknown reason.
> 

As you can see, none of these goals have been achieved *yet*. Right now i’m only more than 1 Month into this project, so don’t expect much. 

## **IMPORTANT**

- I’m currently working on 2 machines. Since i only have a cross compiler on 1, i cannot test the code the other machine. Untested commits are marked with (**UNTESTED**).
- The project is far from done. Keep that in mind. A lot of sudden changes can happen quickly. Don’t expect everything to work flawlessly even if I’m still improving everything over time. I’m trying my best to keep the code as correct and readable as possible.
- If you have suggestions, just put them directly inside ISSUES (I’ll manage issues later. I haven’t set them up yet). I will like any kind of attention i will get. Just a sign that someone sees my project is already a good validation for me.
- But don’t go judging this project too much; I didn’t actually have any bare metal or C experience before, like, at all. I just had the balls to try this out and it somehow worked. But don’t think there wasn’t any pain experienced. There was plenty.

## Project tree:

`/Boot` contains the **Architecture specific boot codes**; they only give enough for the system to boot into C code.

`/Drivers` contains all the **specific Hardware code,** as well as **Generic API’**s for the kernel to communicate with the drivers. 

`/Init` is a subsystem that contains **Hardcoded Driver selections** (chosen at compile time) for each Architecture and Device supported.

`/Kernel` contains all of the Kernel logic code.

`/Libraries` are well, Libraries. Take a look for yourself at the contents. 

## Project Structure:

- Folders start Capital letters, files start with lowercase letters.
- Every `.h` file lives inside a **Global** folder. Everything visible to other TU’s must live inside a header file.
- The Drivers are **Device** oriented. That means, that you search after your device family, not Architecture. Only Init and Boot are Architecture oriented, as they must run on every device that suport that Architecture. This also makes for an easier Build system.