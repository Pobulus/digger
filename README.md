# digger
`ncursesw` based game about digging with emojis

![digger](https://github.com/Pobulus/digger/assets/32367046/8f04e347-742e-4a00-8444-8bac75566a41)

## Compiling
Make sure you have ncurses headers installed. 

On Debian/Ubuntu they can be installed with:
`sudo apt install libncurses-dev`

Then you can compile it using g++:
`g++ main.cpp -lncursesw -o digger`

## Controlls
- `arrow keys` controll movement
- `spacebar` interact with buildings
- `Q/W/E/R` jump to the hospital/restaurant/bank/shop (only overground)
- `X` give up, you instantly return to the surface but your inventory is cleared
- `1` use coffee
- `2` use warpstar
- `esc` exit the game
## Gameplay
The goal of the game is to dig deep underground for valuable resources and return with them to the surface to sell them for profit. 
### Stats
- 🚩 **depth**: your position below ground
- $ **funds**: how much money is in your account
- ❤️ **health**: when it reaches 0, you return to the surface but loose all everything in your inventory
- ⚡ **energy** ⚡: used up when mining. When it reaches 0 you will become exhausted and moving will cause your health to drop
### Buildings
- ⚕️ **hospital**: restores your health to 100 (free of charge)
- 🍽️ **restaurant**: restores your energy for money
- 💰 **bank**: buys valuables from you
- ⚒️ **store**: offers upgrades
### Upgrades and items
![store](https://github.com/Pobulus/digger/assets/32367046/212fd57a-ef78-41ae-8625-05616090a638)

Press the corresponding number to purchase an upgrade. Press `Q` to quit the store. 
1. ⛏️ **pickaxe**: higher level allows you to dig through tougher stone
2. 👷 **protection**: increases the chance to avoid damage while exhausted
3. 🎒 **storage**: number of slots for valuables in you inventory
4. ⚡ **energy**: increases maximum energy capacity

The store also offers usable items

5. ☕ **coffee**: restores half of your energy but lowers your health by 10
6. ✴️ **warp star**: teleports you back to the surface with your inventory intact
