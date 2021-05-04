const r = parseInt(process.argv[2]) || 10;

for (let y = -r; y <= r; y ++) {
    let tot = "";
    for (let x = -r * 2; x <= r * 2; x ++) {
        if (Math.abs(x ** 2 / 4 + y ** 2 - r ** 2) < r * 2 / 3) {
            tot += "#";
        } else {
            tot += " ";
        }
    }
    console.log(tot);
}