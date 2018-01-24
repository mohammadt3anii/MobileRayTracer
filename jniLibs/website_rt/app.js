'use strict';

let express = require('express');
let todoController = require('./Controllers/TodoController');
let homeController = require('./Controllers/HomeController.js');
let raytracerController = require('./Controllers/RayTracerController');
let path = require('path');

let expressApp = express();

//set up template engine
expressApp.set('view engine', 'ejs');

//static files
expressApp.use('/', express.static('./Public'));

//Change Views folder
expressApp.set('views', path.join(__dirname, './Views'));

//fire controllers
todoController(expressApp);
homeController(expressApp);
raytracerController(expressApp);

//listen to port
const port = 60009;
expressApp.listen(60009, '0.0.0.0');
console.log('You are listening to port ' + port);