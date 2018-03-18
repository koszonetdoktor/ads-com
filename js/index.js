const worker = require("streaming-worker");
const through = require('through');
const path = require("path");

const addonPath = path.join(__dirname, "../build/Release/ads_com");
const myWorker = worker(addonPath, ""); // I dont path any options argument

myWorker.to.emit("value", 10);
myWorker.from.on("integer", function(resp) {
    console.log("event response: ", resp);
})

myWorker.from.on("name", function(resp) {
    console.log("response_name: ", resp);
})

const out = myWorker.from.stream();
out.pipe(through(function (data) {
    // the data coming in is an array, 
    // Element 0 is the name of the event emitted by the addon (position_sample)
    // Element 1 is the data - which in this case is 
    this.queue(data[1]);
}))
.pipe(through(function(resp) {
    console.log("stream response: ", resp)
}));

console.log("juhuh")