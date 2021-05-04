const fs = require("fs");

const strings = [
    "What would you like to do?",
    "Welcome to clam's daring jailbreak! Please keep your hands and feet inside the jail at all times.",
    "look around",
    "You look around your cell and you see an old bed along with a snake. Outside, kmh stares at you, wondering how to make the jail cell more contrived.",
    "You're speaking nonsense. Cut that out.",
    "It seems that clam won't be escaping today.",
    "sleep",
    "You lie down on the bed and close your eyes. You then get bitten by the snake. What did you expect to happen when you went to bed next a snake?",
    "You look around your cell and you see an old bed along with a snake. Outside, kmh is nowhere to be seen.",
    "knock on the wall",
    "You here a muffled voice from the other side saying \"I shouldn't have blatantly ripped off pico's challenges...\"",
    "pry the bars open",
    "You start prying the prison bars open. Realizing this is unintended, kmh decides to patch the universe interpreter to make the bars immovable.",
    "You start prying the prison bars open. A wide gap opens and you slip through.",
    "throw the snake at kmh",
    "pick the snake up",
    "You pick the snake up.",
    "You throw the snake at kmh and watch as he runs in fear.",
    'You look around and see that kmh has already made the jail contrived! There\'s a red button and a green button with a sign that says "pres butons 2 get fleg".',
    "press the red button",
    "press the green button",
    "You pressed the red button. Nothing changed.",
    "You pressed the green button. Nothing changed.",
    "bananarama",
    "For some reason, a flag popped out of the wall, and you walk closer to read it.",
    "flag.txt",
    "r",
    "Couldn't find flag file.",
    "Attached to the flag is a key to the front door. It looks like clam is escaping after all.",
];

function encode(v, i) {
    return v.split``
        .map((x) => x.charCodeAt(0))
        .reduce(
            ([acc, l], val) => [(acc * val + 17 * i) % 256, [...l, val ^ acc]],
            [i, []]
        )[1];
}

const encoded = strings.flatMap((v, i) => encode(v, i));
const offsets = [...strings, ""].reduce(
    ([acc, l], val) => [acc + val.length, [...l, acc]],
    [0, []]
)[1];

function arrayize(v, padding = 2) {
    return v
        .map((x) => "0x" + x.toString(16).padStart(padding, "0"))
        .join(", ");
}

const filecontents = `typedef unsigned char byte;
byte encstrings[] = {${arrayize(encoded)}};
int offsets[] = {${arrayize(offsets, 4)}};\n`;

fs.writeFileSync("encstrings.h", filecontents, "utf8");
