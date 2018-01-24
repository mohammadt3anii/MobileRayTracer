'use strict';

let bodyParser = require ('body-parser');
let mongoose = require ('mongoose');

//Connect to the database
mongoose.connect('mongodb://Puscas:123456@ds111608.mlab.com:11608/todos');

//Create a schema - this is like a blueprint
let todoSchema = new mongoose.Schema({
  item: String
});

let TodoModel = mongoose.model('Todo', todoSchema);
/*let itemOne = TodoModel({item: 'test'}).save(function (error) {
  if (error) {
    throw error;
  }
  console.log('Item saved');
});*/

let urlEncodedParser = bodyParser.urlencoded({extended: false});

//let data = [{item: 'get milk'}, {item: 'walk dog'}, {item: 'kick some coding ass'}];

module.exports = function (app) {

  app.get('/todo', function(req, resp) {
    //get data from mongodb and pass it to view
    TodoModel.find({}, function (error, data) {
      if (error) {
        throw error;
      }
      resp.render('Todo_list', {todos: data});
    });
  });

  app.post('/todo', urlEncodedParser, function(req, resp) {
    //get data from the view and add it to mongodb
    let newTodo = TodoModel(req.body).save(function (error, data) {
      if (error) {
        throw error;
      }
      resp.json(data);
    })
  });

  app.delete('/todo/:item', function(req, resp) {
    //delete the requested item from mongodb
    TodoModel.find({item: req.params.item.replace(/\-/g, " ")}).remove(function (error, data) {
      if (error) {
        throw error;
      }
      resp.json(data);
    });
  });

};