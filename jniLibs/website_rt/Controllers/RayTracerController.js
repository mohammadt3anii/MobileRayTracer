'use strict';

const bodyParser = require ('body-parser');
const ref = require('ref');
const ffi = require('ffi');
const arrayType = require('ref-array');
const path = require('path');
const Threads = require('webworker-threads');
const EventEmitter = require('events');
const fs = require('fs');
const rtOutput = fs.createWriteStream('/dev/null', {flags: 'w'}, {fd: 1} );
const unsigned = ref.types.uint32;
const UIntArray = arrayType(unsigned);

//process.stdout.write = rtOutput.write.bind(rtOutput);

//void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator, bool printStdOut) {
const rayTracer = ffi.Library('../libC_Wrapper', {
  'RayTrace': [ 'void', [ UIntArray, 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'bool'] ]
});

const urlEncodedParser = bodyParser.urlencoded({extended: false});



let width = 0;
let height = 0;
let stride = 0;
let threads = 0;
let shader = 0;
let scene = 0;
let samplesPixel = 0;
let samplesLight = 0;
let repeats = 0;
let accelerator = 0;
let bitmap = new UIntArray (1);


module.exports = function (expressApp) {

  expressApp.get('/raytracer', function(req, resp) {
    resp.render('RayTracer');
  });

  expressApp.get('/raytracer/render', function(req, resp) {
    //console.log('3 - 1st pixel green = ', (bitmap[0] >> 8) & 0xFF);
    console.log('3 - 1st pixel - ' + (bitmap[0]));
    const sendToClient = {width: width, height: height, pixelsColor_server: bitmap};
    resp.json(sendToClient);
  });

  expressApp.get('/raytracer/shaders/vertexShader', function(req, resp) {
    resp.sendFile(path.resolve(__dirname  + '/../Public/Assets/Shaders/VertexShader.glsl'));
  });

  expressApp.get('/raytracer/shaders/fragmentShader', function(req, resp) {
    resp.sendFile(path.resolve(__dirname  + '/../Public/Assets/Shaders/FragmentShader.glsl'));
  });

  expressApp.post('/raytracer', urlEncodedParser, function(req, resp) {
    console.log('REQUEST RECEIVED');
    width = req.body['Resolution'];
    height = req.body['Resolution'];
    stride = 0;
    threads = 1;
    shader = req.body['Shader'];
    scene = req.body['Scene'];
    samplesPixel = 1;
    samplesLight = 1;
    repeats = 1;
    accelerator = req.body['Accelerator'];
    bitmap = new UIntArray (width * height);

    console.log('body = ' + JSON.stringify(req.body));
    for(let i = 0; i < bitmap.length; i++) {
      bitmap[i] = 0xFF00FF00;
    }

    console.log('1 - 1st pixel - ' + (bitmap[0]));
    //console.log('1 - 1st pixel green = ', (bitmap[0] >> 8) & 0xFF);

    //void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator) {
    rayTracer.RayTrace(bitmap, width, height, stride, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator, false);

    console.log('2 - 1st pixel green = ', (bitmap[0] >> 8) & 0xFF);

    //console.log('bitmap = ' + bitmap);
    console.log('size = ' + bitmap.length);

    //ABGR
    const sendToClient = {width: width, height: height, pixelsColor_server: bitmap};
    resp.json(sendToClient);

    var worker = new Threads.Worker(function(){
      this.onmessage = function(data) {
        console.log('WORKER STARTED');
        //console.log('WORKER' + JSON.stringify(data));
        console.log('WORKER - ' + (data.data.param1[0]));
        //rayTracer.RayTrace(event.bitmap, width, height, stride, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator, false);
        self.close();
        console.log('WORKER FINISHED');
      };
    });
    worker.postMessage({param1: bitmap.toJSON()});
    console.log('RESPONDED');
  });

};