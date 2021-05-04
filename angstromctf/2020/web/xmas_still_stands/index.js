const express = require("express");
const exphbs = require("express-handlebars");
const bodyParser = require("body-parser");
const path = require("path");
const puppeteer = require("puppeteer");
const request = require("request-promise");
const cookieParser = require("cookie-parser");

const app = express();
const port = process.env.PORT || 60600;

let adminCookie = "hello_yes_i_am_admin";

const admins = ["John", "Jeff", "Jane", "Jason", "JJ", "James", "Jay", "Joe", "Jacob", "Juan", "Jack", "Jonah", "Jamie", "Jenna"];

app.use(express.static(path.join(__dirname, "public")));

app.use(cookieParser());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());
app.use(bodyParser.raw());

const hbs = exphbs.create({
    extname: ".hbs",
    helpers: {
        nonNegative(n) {
            return n >= 0;
        },
        postURL(n) {
            return `/posts/${n}`;
        },
        captchaKey() {
            return process.env.CAPTCHA_SITE;
        }
    }
});

const posts = {};
let pid = 0;

app.engine("hbs", hbs.engine);
app.set("view engine", "hbs");
app.set("views", path.join(__dirname, "views"));

app.get("/", function(req, res) {
    res.render("home");
});
app.get("/post", function(req, res) {
    res.render("post", {
        postid: -1
    });
});
app.post("/post", function(req, res) {
    if (!req.body.content) {
        res.status(400).render("post", {
            err: "Please actually post something."
        });
        return;
    }
    if (typeof req.body.content != "string") {
        res.status(400).render("post", {
            err: "Nice try but not funny."
        });
        return;
    }
    if (req.body.content.length < 3 || req.body.content.length > 200) {
        res.status(400).render("post", {
            err: "We only accept posts with 3-200 characters."
        });
        return;
    }
    let ind = pid ++;
    posts[ind] = {
        time: Date.now(),
        content: req.body.content
    };
    res.render("post", {
       postid: ind
    });
})
app.get("/report", function(req, res) {
    res.render("report");
});
app.get("/admin", function(req, res) {
    res.render("admin", {
        isAdmin: req.cookies.super_secret_admin_cookie == adminCookie,
        flag: process.env.FLAG
    });
});
app.get("/api/posts/:id", function(req, res) {
   if (req.params.id in posts) {
       res.send(posts[req.params.id].content);
   } else {
       res.sendStatus(404);
   }
});
app.get("/posts/:id", function(req, res) {
    res.render("viewpost");
});

async function visitPost(id) {
    const browser = await puppeteer.launch({
        args: (process.env.CHROME_FLAGS || "").split`|`
    });
    const page = await browser.newPage();
    const name = admins[Math.floor(Math.random() * admins.length)];
    const dom = `127.0.0.1:${port}`;
    await page.setCookie({
        name: "super_secret_admin_cookie",
        value: adminCookie,
        httpOnly: false,
        sameSite: "Lax",
        domain: dom
    }, {
        name: "admin_name",
        value: name,
        httpOnly: false,
        sameSite: "Lax",
        domain: dom
    });
    await page.setUserAgent(`${name}'s browser`);
    await page.goto(`http://${dom}/posts/${id}`, {waitUntil: "networkidle0"});
    await page.close();
    await browser.close();
    delete posts[id];
}

app.post("/report", async function(req, res) {
    const captcha = req.body["g-recaptcha-response"];
    if (!captcha) {
        res.status(400).render("report", {
            err: "Please complete the CAPTCHA."
        });
        return;
    }
    if (process.env.CAPTCHA_SECRET) {
        const verification = JSON.parse(await request({
            uri: "https://www.google.com/recaptcha/api/siteverify",
            method: "POST",
            form: {
                secret: process.env.CAPTCHA_SECRET,
                response: captcha
            }
        }));
        if (!verification.success) {
            res.status(400).render("report", {
                err: "Invalid CAPTCHA. Please try again."
            });
            return;
        }
    }
    const id = Number(req.body.id);
    if (!Number.isInteger(id) || id < 0) {
        res.status(400).render("report", {
            err: "Please provide an actually valid ID."
        });
        return;
    }
    if (!(id in posts)) {
        res.status(400).render("report", {
            err: "Could not find post. It may have expired or been deleted by an admin."
        });
        return;
    }
    res.render("report", {
        succ: "We will be checking out the post shortly."
    });
    visitPost(id);
});

app.use(function(req, res) {
    res.status(404).render("error", {
        title: "404",
        msg: "Page not found."
    });
})

setInterval(function() {
    for (let postid in posts) {
        if (Date.now() > posts[postid].time + 1000 * 60 * 15) {
            delete posts[postid];
        }
    }
}, 10000);

async function main() {
    app.listen(port, function() {
        console.log(`App listening on port ${port}.`);
    });
}
main();