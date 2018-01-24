'use strict';

const bodyParser = require ('body-parser');
const ref = require('ref');
const ffi = require('ffi');
const arrayType = require('ref-array')
const path = require('path');
const Threads = require('webworker-threads');

const unsigned = ref.types.uint32;
const UIntArray = arrayType(unsigned);

//void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator) {
const rayTracer = ffi.Library('../libC_Wrapper', {
  'RayTrace': [ 'void', [ UIntArray, 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int'] ]
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
    
    //void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator) {
    rayTracer.RayTrace(bitmap, width, height, stride, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator);

    //console.log('bitmap = ' + bitmap);
    console.log('size = ' + bitmap.length);

    //ABGR
    const sendToClient = {width: width, height: height, pixelsColor_server: bitmap};
    resp.json(sendToClient);

    //rayTracer.RayTrace(bitmap, width, height, stride, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator);
    /*var worker = new Threads.SharedWorker(function(){
      //postMessage("I'm working before postMessage('ali').");
      rayTracer.RayTrace(bitmap, width, height, stride, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator);
      expressApp.get('/raytracer/render', function(req, resp) {
        const sendToClient = {width: width, height: height, pixelsColor_server: bitmap};
        resp.json(sendToClient);
      });
      /*this.onmessage = function(event) {
        postMessage('Hi ' + event.data);
        self.close();
      };
    });*/
    /*worker.onmessage = function(event) {
      console.log("Worker said : " + event.data);
    };
    worker.postMessage('ali');*/
    console.log('RESPONDED');
  });

};