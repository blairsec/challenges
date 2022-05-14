const express = require("express");
const path = require("path");

const app = express();
const port = Number(process.env.PORT) || 8080;

app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "index.html"));
});

app.listen(port, () => {
    console.log(`Server listening on port ${port}.`);
});
