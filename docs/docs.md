# Work with the DisRealityGF
- Add a class that derived dr::Engine class from DisRealityGF which contain the game cycle.
```
#include <DisRealityGF.h>
class GameEngine : public dr::Engine
{
};
```
- Run a derived run() method to start a game cycle.
```
GameEngine gameEngine;
gameEngine.run();
```
- Override  the virtual dr::init() method from the DisRealityGF.

File's structure

The folder structure for the assets:
assets - in the root folder
assets -> img -- images
assets -> font -- fonts
assets -> sound -- sounds

data -> texture_list.ini - pairs of texture's id and a path to texture's file