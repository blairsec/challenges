const flag = "Blockchain big data solutions now with added machine learning. Enjoy! I sincerely hope you actf{wr0t3_4_pr0gr4m_t0_h3lp_y0u_w1th_th1s_df93171eb49e21a3a436e186bc68a5b2d8ed} instead of doing it by hand.";
const funcsPerChar = 5;
const tot = flag.length * funcsPerChar;
const rand = [];
for (let i = 0; i < tot; i ++) {
    rand.push(i);
}
let randlen = rand.length;
function getNum() {
    const i = Math.floor(Math.random() * randlen);
    const temp = rand[i];
    rand[i] = rand[--randlen];
    rand[randlen] = temp;
    return temp;
}
let chars = "";
for (var i = 0x20; i <= 0x7f; i ++) {
    chars += String.fromCharCode(i);
}
function randChar() {
    return chars[Math.floor(Math.random() * chars.length)];
}
function format(n, i, c) {
    return `b f${n}(s x){return x[${i}]==${c.charCodeAt(0)};}`;
}
const funcs = [];
const nums = [];
for (let i = 0; i < flag.length; i ++) {
    const cur = flag[i];
    const curNum = getNum();
    nums.push(curNum);
    funcs.push(format(curNum, i, cur));
    for (let j = 0; j < 4; j ++) {
        funcs.push(format(getNum(), i, randChar()));
    }
}
console.log(`#include <stdio.h>
#define b int
#define s char *
${funcs.sort(_ => Math.random() - 0.5).join``}
char z[256];
int main() {
    printf("Give me some training data:\\n");
    fgets(z, 256, stdin);
    if (${nums.sort(_ => Math.random() - 0.5).map(v => `f${v}(z)`).join`&&`}) {
        printf("Hmm yes this is some fine training data.\\n");
    } else {
        printf("Nah I don't like this training data.\\n");
    }
    return 0;
}`);
