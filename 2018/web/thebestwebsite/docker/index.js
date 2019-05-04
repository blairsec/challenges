var fs = require('fs');
var express = require("express")
var app = express()

// from https://developer.mozilla.org/en-US/docs/Learn/Server-side/Express_Nodejs/mongoose
//Import the mongoose module
var mongoose = require('mongoose');

//Set up default mongoose connection
var mongoDB = 'mongodb://localhost/problem';
mongoose.connect(mongoDB);

//Get the default connection
var db = mongoose.connection;

//Bind connection to error event (to get notification of connection errors)
db.on('error', console.error.bind(console, 'MongoDB connection error:'));

var objectSchema = new mongoose.Schema({
  data: String
})

var object = mongoose.model("Object", objectSchema)

mongoose.connection.collections['objects'].drop( function(err) {
    console.log('collection dropped');
});

log = "Sat Aug 10 2017 10:23:17 GMT-0400 (EDT) - Initial website\nSat Aug 10 2017 14:54:07 GMT-0400 (EDT) - Database integration\nSat Aug 11 2017 14:08:54 GMT-0400 (EDT) - Make some changes to the text\n"

var box1 = new object({data: "Go away.^This website has literally nothing of interest. You might as well leave."})
var box2 = new object({data: "You will be very bored.^Seriously, there's nothing interesting."})
var box3 = new object({data: "Please just leave.^Scrolling more will only give you more boring content."})
box1.save()
box2.save()
box3.save()

var fs = require('fs')
fs.readFile("public/js/init.js", 'utf8', function (err,data) {
  var result = data.replace(/ids = \[.*,.*,.*\]/g, "ids = [\""+box1._id+"\",\""+box2._id+"\",\""+box3._id+"\"]");

  fs.writeFile("public/js/init.js", result, 'utf8', function (err) {
  });
});

setTimeout(function() {
  var flag = "actf{0bj3ct_ids_ar3nt_s3cr3ts}"
  var flago = new object({data: flag})
  console.log(flago._id)
  flago.save()
  fs.writeFile("public/log.txt", log+flago._id.getTimestamp()+" - Add super secret flag to database", function(err) {});
}, 6000)

app.get("/boxes", (req, res) => {
  try {
  console.log("getting boxes")
  var foundObjects = []
  var ids = req.query.ids.split(",")
  if (ids.length != 3) {
    return res.status(400).send("number of ids does not equal 3")
  }
  object.findOne({_id: ids[0]}, function(error, box) {
    foundObjects.push(box)
    object.findOne({_id: ids[1]}, function(error, box) {
      foundObjects.push(box)
      object.findOne({_id: ids[2]}, function(error, box) {
        foundObjects.push(box)
        res.status(200).send(JSON.stringify({boxes: foundObjects}))
      })
    })
  })} catch (e) {
 res.status(500).send("error") 
 }
})


app.use(express.static("public"))

app.listen(7667)
