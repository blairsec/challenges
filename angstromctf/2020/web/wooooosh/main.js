var socket = io();
var bStart = document.getElementById("bStart");
var cGame = document.getElementById("cGame");
var pScore = document.getElementById("pScore");
var score = 0;
var msg = "";
var shapes = [];
var ctx = cGame.getContext("2d");
function update() {
    pScore.innerHTML = `Score: ${score}${msg ? ", " : ""}${msg}`;
}
function drawShapes() {
    ctx.clearRect(0, 0, 500, 300);
    shapes.map((v, i) => i ? ctx.fillRect(v.x - 5, v.y - 5, 10, 10) : (ctx.beginPath() + ctx.arc(v.x, v.y, 5, 0, Math.PI * 2) + ctx.closePath() + ctx.fill()));
}
function getCursorPosition(canvas, event) {
    const rect = canvas.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;
    return [x, y];
}
bStart.addEventListener("click", function() {
    socket.emit("start");
});
socket.on("disconnect", function() {
    msg = "You have been disconnected.";
    update();
});
socket.on("score", function(sc) {
    score = sc;
    update();
});
socket.on("disp", function(m) {
    msg = m;
    update();
});
socket.on("shapes", function(s) {
    shapes = s;
    drawShapes();
});
ctx.fillStyle = "#ff0000";
cGame.addEventListener("click", function(e) {
    var pos = getCursorPosition(cGame, e);
    socket.emit("click", pos[0], pos[1]);
});