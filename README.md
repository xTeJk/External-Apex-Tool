# External Apex Tool
[![Discord](https://img.shields.io/discord/748288505507217428.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/AEfuvwT)
  
## Infos:
Driver is detected and 2 cheat games will result in your account being banned!  
Glow is based on:
  - [NUM1] > Life + Shield 
    - light pink: 1-50,
	- pink: 50-100,
	- purple: 80-120,
	- dark purple: 120-150,
	- blue: 150-180,
	- dark blue: 180-200
  - [NUM2] > Vision 
    - darker: can't see enemy,
	- lighter: can see enemy
  
## How to:
- Use VS 2019  
- Install WDK & SDK  
  - Make sure that /Qspectre is disabled for driver  
  - Build driver and cheat as `Release x64`  
- Map driver using KDMapper (https://github.com/TheCruZ/kdmapper)  
- Launch game  
- Launch UM in menu
  
## Credits:
- Null (driver)  
- AdrianVPL (glow)
  
## Keybinds:
If you want to change keybinds use:  
https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes  
find interesting key `VK_` or `0x` code and put inside `main.cpp` (CheatUM) at correct place, eg.:  
`if (GetAsyncKeyState(VK_NUMPAD1))` -> if (GetAsyncKeyState(VK_NUMPAD5))  
  
## Hooked function:
If you want to change function which is being hooked by driver find:
`NtOpenCompositionSurfaceSectionInfo`  
inside `main.cpp` (CheatUM) & `hook.cpp` (Driver)  
and then change it for something else (https://github.com/hfiref0x/NtCall64/blob/master/Source/NtCall64/tables.h)  
  
Remember! There can't be `__security_cookie` inside, coz its going to cause bsod ;x

## Images:
![img](https://i.imgur.com/cjggaZF.png)  
  
  
glhf