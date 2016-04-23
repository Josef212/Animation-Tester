Welcome to Morpheus Game's animation tester for our Diablo's 2 tribute project.


 Using this tool you can check if your xml animation file is properly filled for our engine, just fill all the gaps in the animations.xml but the ones called "name"!!!


 You can also import your own xml file but it must have the same format as the sample one:
	1 - Add the xml file to animation folder.
	2 - Open config.xml.
	3 - Change the animation_file value on the entity_manager child.


 In order to check animations for diferent texture follow next steps:
	1 - Add the texture to the texture folder.
	2 - Open config.xml file.
	3 - Change the entity_texture value on the test_scene child.


 If you have another map and you want to change the default one you can, but make sure it is in mtx format and you have its tile sets:
	1 - Add the map and tile sets to "maps" folder inside "data.zip".
	2 - Open config.xml file.
	3 - Change the map_name on the test_scene child.


 Once you have introduced the texture and animation you want just play the application.


 Inside the tester you can move the entity arround the map using arrow buttons and change the entity's direction in order to check all animations points of view 
using 1-8 number buttons.

You can press F1 to see the degbug quad that matches with the animation frame (red) and the pivot (green).

Press space bar to reset the animation.

Finally you can exit by pressing ESC.


Josef21296 - Morpheus Games.
www.github.com/Josef212
www.github.com/MorpheusGames