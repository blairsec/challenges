use std::any::Any;
use std::cell::RefCell;
use std::env::args;
use std::fmt::{self, Display, Formatter};
use std::mem::{replace, swap};
use std::panic::{catch_unwind, panic_any, set_hook};

struct S {
    s16: String,
    s17: String,
    s18: String,
    s19: String,
    s20: String,
    s21: String,
    s22: String,
    s23: String,
    s24: String,
    s25: String,
    s26: String,
    s27: String,
    s28: String,
    s29: String,
    s30: String,
    s31: String,
    s00: String,
    s01: String,
    s02: String,
    s03: String,
    s04: String,
    s05: String,
    s06: String,
    s07: String,
    s08: String,
    s09: String,
    s10: String,
    s11: String,
    s12: String,
    s13: String,
    s14: String,
    s15: String,
}

impl Display for S {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        write!(f, "{31}{30}{29}{28}{27}{26}{25}{24}{23}{22}{21}{20}{19}{18}{17}{16}{15}{14}{13}{12}{11}{10}{9}{8}{7}{6}{5}{4}{3}{2}{1}{0}", self.s24, self.s25, self.s26, self.s27, self.s28, self.s29, self.s30, self.s31, self.s16, self.s17, self.s18, self.s19, self.s20, self.s21, self.s22, self.s23, self.s08, self.s09, self.s10, self.s11, self.s12, self.s13, self.s14, self.s15, self.s00, self.s01, self.s02, self.s03, self.s04, self.s05, self.s06, self.s07)
    }
}

pub fn main() {
    let string = args().rev().next().unwrap();
    assert_eq!(&string[..5], "actf{");
    let suffix = &string[5..];
    let mut input = suffix.chars();
    let s = S {
        s03: input.next().unwrap().to_string(),
        s02: input.next().unwrap().to_string(),
        s01: input.next().unwrap().to_string(),
        s00: input.next().unwrap().to_string(),
        s07: input.next().unwrap().to_string(),
        s06: input.next().unwrap().to_string(),
        s05: input.next().unwrap().to_string(),
        s04: input.next().unwrap().to_string(),
        s11: input.next().unwrap().to_string(),
        s10: input.next().unwrap().to_string(),
        s09: input.next().unwrap().to_string(),
        s08: input.next().unwrap().to_string(),
        s15: input.next().unwrap().to_string(),
        s14: input.next().unwrap().to_string(),
        s13: input.next().unwrap().to_string(),
        s12: input.next().unwrap().to_string(),
        s19: input.next().unwrap().to_string(),
        s18: input.next().unwrap().to_string(),
        s17: input.next().unwrap().to_string(),
        s16: input.next().unwrap().to_string(),
        s23: input.next().unwrap().to_string(),
        s22: input.next().unwrap().to_string(),
        s21: input.next().unwrap().to_string(),
        s20: input.next().unwrap().to_string(),
        s27: input.next().unwrap().to_string(),
        s26: input.next().unwrap().to_string(),
        s25: input.next().unwrap().to_string(),
        s24: input.next().unwrap().to_string(),
        s31: input.next().unwrap().to_string(),
        s30: input.next().unwrap().to_string(),
        s29: input.next().unwrap().to_string(),
        s28: input.next().unwrap().to_string(),
    };
    assert_eq!(input.next().unwrap(), '}');
    if let Some(_) = input.next() {
        panic!();
    }
    let actual = format!("{}", catch_unwind(|| {
        let x: RefCell<Box<dyn Any + Send>> = RefCell::new(Box::new(s));
        set_hook(Box::new(|info| {
            let p = info.payload().downcast_ref::<Box<RefCell<Box<dyn Any + Send>>>>().unwrap();
            let old: Box<dyn Any + Send> = replace(&mut *p.borrow_mut(), Box::new(()));
            let mut m = old.downcast::<S>().unwrap();
            swap(&mut m.s00, &mut m.s01);
            swap(&mut m.s02, &mut m.s03);
            swap(&mut m.s04, &mut m.s05);
            swap(&mut m.s06, &mut m.s07);
            swap(&mut m.s08, &mut m.s09);
            swap(&mut m.s10, &mut m.s11);
            swap(&mut m.s12, &mut m.s13);
            swap(&mut m.s14, &mut m.s15);
            swap(&mut m.s16, &mut m.s17);
            swap(&mut m.s18, &mut m.s19);
            swap(&mut m.s20, &mut m.s21);
            swap(&mut m.s22, &mut m.s23);
            swap(&mut m.s24, &mut m.s25);
            swap(&mut m.s26, &mut m.s27);
            swap(&mut m.s28, &mut m.s29);
            swap(&mut m.s30, &mut m.s31);
            let d: Box<dyn Display + Send> = Box::new(m);
            *p.borrow_mut() = Box::new(d);
        }));
        panic_any(Box::new(x));
    }).unwrap_err().downcast::<Box<RefCell<Box<dyn Any + Send>>>>().unwrap().borrow().downcast_ref::<Box<dyn Display + Send>>().unwrap());
    assert_eq!("_ynourtsd_tet_eh2_bfiasl7cedbda7", actual);
    println!("Correct!");
}