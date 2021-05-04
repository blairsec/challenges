const express = require("express");
const path = require("path");
const app = express();

const port = process.env.PORT || 60007;
const flag = process.env.FLAG;

app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "index.html"));
});

app.get("/flag", (req, res) => {
    if (req.query.msg == "please give flag") {
        res.send(flag.split``.map(v => String.fromCharCode(v.charCodeAt(0) ^ 0xf)).join``);
    }
});

app.listen(port, function() {
    console.log(`App listening on port ${port}!`);
});