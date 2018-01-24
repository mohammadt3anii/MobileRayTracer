//counter = reference to module.exports of count.js
let stuff = require('./stuff');

console.log(stuff.counter2(['shaun', 'crystal', 'ryu']));
console.log(stuff.adder(1, stuff.pi));