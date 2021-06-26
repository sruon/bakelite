![EpicSave](https://i.imgur.com/jlTcq5K.png)
![Summary](https://i.imgur.com/8TsdYD8.png)

# Bakelite
A Bakkesmod plugin to support Nvidia Geforce Experience Highlights in Rocket League

> NVIDIA Highlights enables automatic video capture of key moments, clutch kills, and match-winning plays, ensuring gamers’ best gaming moments are always saved. Once a Highlight is captured, gamers can simply share it directly to Facebook, YouTube, or Weibo right from GeForce Experience’s in-game overlay. Additionally, they can also clip their  favorite 15 seconds and share as an animated GIF - all without leaving the game!

## Requirements
- Nvidia GPU
- [Geforce Experience](https://www.nvidia.com/en-us/geforce/geforce-experience/download/)

## Disclaimer
This plugin will download **GfeSDK.dll** in your Rocket League folder. This is the official Nvidia Geforce Experience SDK DLL that can be found in a [gated Nvidia repository](https://github.com/NVIDIAGameWorks/GfeSDK)

If you would like to compare the [.dll downloaded by this plugin](https://github.com/sruon/bakelitegfesdk/tree/main/source/redist) with the [upstream](https://github.com/NVIDIAGameWorks/GfeSDK/tree/master/redist/win64), please follow the steps highlighted [on this page](https://developer.nvidia.com/highlights) so that Nvidia can invite you to the organization and gain access to the repository.

## How to install
TBD

## How to use
### Accept permissions
Upon first loading the plugin, a permission request will be opened by Nvidia Geforce Experience - you must accept the permissions requested (video) in order to proceed.
### Head into a Private Match
When in the match, ensure this icon shows up in the bottom right corner ![Nvidia Shadowplay](https://i.imgur.com/NsgD7mW.png)

If it does, you're good to go!
### Select events to capture
Bakelite comes with a default set of events that will be saved (see **Supported events**) - you may edit this list of event at any time through the Nvidia Shadowplay overlay (ALT+Z) - see **FAQ** for more informations.

### Save your captures
Captures are **NOT SAVED** until you've gone through the **Summary** screen and clicked the **Save** button for each.

Depending on your plugin settings, the **Summary** screen may show automatically when quitting to the menu from a match. 

If you've opted out of this feature, you may simply use the **PgUp** key on your keyboard to load it.

## Supported events
**Note: All events are captured at -5s/+3s at the time**

| Event | Default save |
|-------|--------------|
| Goal  | :white_check_mark:   |
| EpicSave | :white_check_mark:
| Save | :white_check_mark: |
| HighFive | :white_check_mark: |
| Assist | :white_check_mark: |
| OwnGoal | ✅ |
| Demolish | :x: |
| Demolition | :x: |
| Win | :x: |
| MVP | :x: |
| AerialGoal | :x: |
| BackwardsGoal | :x: |
| BicycleGoal | :x: |
| LongGoal | :x: |
| TurtleGoal | :x: |
| PoolShot | :x: |
| OvertimeGoal | :x: |
| HatTrick | :x: |
| Playmaker | :x: |
| Savior | :x: |
| Shot | :x: |
| Center | :x: |
| Clear | :x: |
| FirstTouch | :x: |
| BreakoutDamage | :x: |
| BreakoutDamageLarge | :x: |
| LowFive | :x: |
| HoopsSwishGoal | :x: |
| BicycleHit | :x: |

## FAQ
#### Where are files saved after adding them to the Gallery
Videos\Rocket League

Additionally, the default temporary folder for Nvidia Highlights is **%localappdata%\Temp\Highlights\Rocket League**
#### How do I know Nvidia Geforce Experience is running
- In the bottom right corner of your game, one or multiple icons will be present (you need to be in a match to check - Try a Private Match)
  - ![Nvidia Shadowplay](https://i.imgur.com/NsgD7mW.png)
- ALT+Z should open the Geforce Experience overlay

#### How do I select events to record
- Open the overlay (ALT+Z)
- Open _Settings_ (Cog symbol)
- Open _Highlights_
- Below _Rocket League_
  - **Enable** (green) any event you wish to capture
  - **Disable** (grey) any event you wish to ignore
  - NOTE: These settings are saved between sessions
  - **NOTE2: You must have entered at least 1 match before the settings will appear - Try a Private Match**

#### How do I disable my microphone capture
- Open the overlay (ALT+Z)
- Click the _Microphone_
- Select **Off**

## Issues
#### Shadowplay not recording
- Try reloading the plugin
- Try forcing the Geforce Experience overlay to appear - ALT+Z
- Ensure the icon is appearing in the bottom right corner while in a match
- Ensure Nvidia Highlights is enabled
  - ALT+Z
    - Settings (Cog symbol)
      - Highlights
        - Capture **enabled** (green)
        - Rocket League **enabled** (green)

#### Shadowplay capturing desktop / other monitors
- Open the overlay (ALT+Z)
- Open _Settings_ (Cog symbol)
- Open _Privacy Control_ (near the bottom)
- _Desktop capture_ **disabled** (grey)

#### Shadowplay overlay is appearing for a split second on first capture
Unsure on how to fix that yet.

## TODO
- Allow customization of keybind for Geforce Experience highlight summary page trigger
- Expose events through .json to allow timers customization
- Freeplay mode support with goals disabled
- https://bakkesplugins.com listing

## Credits
[ubelhj/OBSCounter](https://github.com/ubelhj/OBSCounter) for stats events and hook functions 

[blaku-rl/CustomBindingPlugin](https://github.com/blaku-rl/CustomBindingPlugin) for keypress handling

[NVIDIAGameWorks/GfeSDK](https://github.com/NVIDIAGameWorks/GfeSDK) for Geforce Experience SDK and sample code
