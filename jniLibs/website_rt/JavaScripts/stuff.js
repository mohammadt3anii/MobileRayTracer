var counter = function (array) {
  return 'There are ' + array.length + ' elements in this array';
};

var adder = function (a, b) {
  return `The sum of the 2 numbers is ${a+b}`;
};

var pi = 3.142;

module.exports = {
  counter2: counter,
  adder: adder,
  pi: pi
};