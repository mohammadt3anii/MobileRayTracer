'use strict';

const events = require('events');
const util = require('util');

//event.on('click', function(){});

const myEmitter = new events.EventEmitter();

myEmitter.on('someEvent', function(mssg) {
  console.log(mssg);
});

myEmitter.emit('someEvent', 'the event was emitter');

const Person1 = function (name) {
  this.name = name;

  this.getName = function () {
    return this.name;
  };
};

class Person2 extends events.EventEmitter {

  constructor (name) {
    super();
    this.name = name;
  }

  get name () {
    return this.name_;
  }

  set name (value) {
    if (value.length < 4) {
      console.log("Name is too short.");
      return;
    }
    this.name_ = value;
  }

  getName () {
    return this.name;
  }
}

util.inherits(Person1, events.EventEmitter);

const james = new Person2('james');
const mary = new Person1('mary');
const ryu = new Person1('ryu');
const people = [james, mary, ryu];
people.forEach(function(person) {
  person.on('speak', function(mssg) {
    let chat = document.getElementById('Chat');
    chat.innerText += person.getName() + ' said: ' + mssg + '\n';
  });
});

james.emit('speak', 'hey dudes');
ryu.emit('speak', 'I want a curry');