# cocktailbot
WIO Terminal based cocktail mixing robot


Untested as of now. (still building the hardware)

Delivers a signal to relais or mosfets to drive up to 11 (or more, if you adjust the code) peristaltic pumps - up to 3 are active at the same time (Can be adjusted in Code, depends on the current your power source can deliver). The flow is monitored for empty-bottle or other fail-detection (untested). Also the content of each flask is estimated (but nothing happens with this info yet).

flask-names, flowcalibrationdata and recipes are read from customizable txt files on an SD-card at startup so that there is a no programming approach to modify recipes etc.

Leaves space for unknown ingredients in the recipes - good for filling up a premix with the main ingredient as the peristaltic pumps can be slow.
cocktail-size can be adjusted at runtime.
