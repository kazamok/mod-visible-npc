# mod-visible-npc

This is a custom AzerothCore module that allows you to control the visibility of specific NPCs via the worldserver configuration.

## 🔧 Features

- Hide/show specific NPCs based on entry ID.
- Configurable in `worldserver.conf` or `mod_visible_npc.conf`.
- Easy to maintain and extend.

## 📁 Files

- `src/mod_visible_npc.cpp` – Core logic
- `conf/mod_visible_npc.conf.dist` – Sample config file
- `CMakeLists.txt` – Module build config

## ⚙️ Configuration

Add the following to your `worldserver.conf` (or override using a separate conf):

```ini
# NPC visibility control
VisibleNPC.Enable = 1         # 1 = visible, 0 = hidden
VisibleNPC.Entry = 123456     # Entry ID of the target NPC
