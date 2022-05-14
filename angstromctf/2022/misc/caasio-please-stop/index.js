#!/usr/local/bin/node

// flag in ./flag.txt

const vm = require("vm");
const readline = require("readline");

const interface = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
});

interface.question(
    "Welcome to CaaSio: Please Stop Edition! Enter your calculation:\n",
    function (input) {
        interface.close();
        if (
            input.length < 215 &&
            /^[\x20-\x7e]+$/.test(input) &&
            !/[.\[\]{}\s;`'"\\_<>?:]/.test(input) &&
            !input.toLowerCase().includes("import")
        ) {
            try {
                const val = vm.runInNewContext(input, {});
                console.log("Result:");
                console.log(val);
                console.log(
                    "See, isn't the calculator so much nicer when you're not trying to hack it?"
                );
            } catch (e) {
                console.log("your tried");
            }
        } else {
            console.log(
                "Third time really is the charm! I've finally created an unhackable system!"
            );
        }
    }
);
