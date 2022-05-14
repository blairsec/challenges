var form = document.getElementById("form");
var content = document.getElementsByName("paste_content")[0];

var socket = io();

socket.on("connect", function() {
    console.log("Connected to server");
});

function save_changes() {

    var name = document.getElementById("username").value;
    var session = document.getElementById("session").value;

    socket.emit("update", {
        new_content: content.value,
        username: name,
        sess: session
    });
}

content.addEventListener("input", save_changes);