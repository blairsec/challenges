const readline = require("readline");
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
rl.question("", function(v) {
    if (v == "clamclam") {
        console.log("actf{cl4m_is_my_f4v0rite_ctfer_in_th3_w0rld}");
        process.exit(0);
    }
});
let prints = ["clam{clam_clam_clam_clam_clam}", "malc{malc_malc_malc_malc_malc}"];
let printi = 0;
setInterval(function() {
    let str = "";
    if (Math.random() < 0.01) {
        str = `type "clamclam" for salvation\r`;
    }
    console.log(str + prints[printi]);
    printi ++;
    printi %= prints.length;
}, 10);
