# cocktailbot
WIO Terminal based cocktail mixing robot


Untested as of now.

Uses up to 11 peristaltic pumps - up to 3 are active at the same time (Can be adjusted in Code). The flow is monitored for empty-bottle or other fail-detection (untested).

flask-names, flowcalibrationdata and recipes are read from customizable txt files on an SD-card at startup.

Leaves space for unknown ingredients in the recipes.
cocktail-size can be adjusted at runtime.
