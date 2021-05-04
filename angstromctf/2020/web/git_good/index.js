const express = require("express");
const app = express();

const port = process.env.PORT || 9090;

app.use(express.static("."));

app.listen(port, function() {
    console.log(`App listening on port ${port}!`);
})