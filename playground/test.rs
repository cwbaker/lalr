//
// Here I've added a space in generic parameter that end in ">>" to allow parse it
//
#![allow(unused_must_use)]

extern crate syntax;
extern crate rustc;
extern crate serialize;
extern crate getopts;
extern crate rustc_trans;
extern crate rustc_driver;

use getopts::{optflag,getopts};

use std::io;
use std::os;
use std::io::Reader;
use std::string::String;
use serialize::json;
use serialize::json::{Json};
use syntax::diagnostics::registry;

use rustc::session;
use rustc::session::config;
use rustc::session::config::Input;

use rustc_driver::driver;

fn filter_json(j: &mut json::Json) {
    let mut replace = None;

    match *j {
        Json::Object(ref mut ob) => {
            // remove
            ob.remove(&String::from_str("span"));
            ob.remove(&String::from_str("id"));
            let mut kv : Option<(String,Vec<Json>)> = None;
            match ob.get(&String::from_str("node")) {
                Some(&Json::Object(ref ob2)) => {
                    match (ob2.get(&String::from_str("variant")),
                           ob2.get(&String::from_str("fields"))) {
                        (Some(&Json::String(ref s)), Some(&Json::Array(ref ls))) => {
                            kv = Some((s.clone(), ls.clone()));
                        }
                        _ => ()
                    }
                }
                _ => ()
            }
            match kv {
                None => (),
                Some((k, v)) => {
                    ob.remove(&String::from_str("node"));
                    ob.insert(k, Json::Array(v));
                }
            }
            for (_, v) in ob.iter_mut() {
                filter_json(v);
            }
        }
        Json::Array(ref mut ls) => {
            for v in ls.iter_mut() {
                filter_json(v)
            }
            if ls.len() == 1 {
                replace = Some(ls.as_slice()[0].clone())
            }
        }
        _ => ()
    }
    match replace {
        None => (),
        Some(jj) => *j = jj
    }
}

static INDENT_STEP: int = 4;

fn print_indent(indent: int) {
    let mut out = io::stdout();
    for i in range(0, indent) {
        if i % INDENT_STEP == 0 {
            out.write_str("|");
        } else {
            out.write_str(" ");
        }
    }
}

fn print_sexp(indent: int, j: &json::Json) {
    let mut out = io::stdout();

    match *j {

        Json::Object(ref ob) => {
            {
                for (k, v) in ob.iter() {
                    print_indent(indent);
                    out.write_str("(");
                    out.write_str(k.as_slice());
                    out.write_str("\n");
                    print_sexp(indent + INDENT_STEP, v);
                    print_indent(indent);
                    out.write_str(")\n");
                }
            }
        }
        Json::Array(ref ls) => {
            print_indent(indent);
            out.write_str("(\n");
            for v in ls.iter() {
                print_sexp(indent + INDENT_STEP, v);
            }
            print_indent(indent);
            out.write_str(")\n");
        }
        Json::String(ref s) => {
            print_indent(indent);
            out.write_str(s.as_slice());
            out.write_str("\n");
        }
        Json::Null => {
            print_indent(indent);
            out.write_str("<nil>\n");
        }
        Json::Boolean(true) => {
            print_indent(indent);
            out.write_str("true\n");
        }
        Json::Boolean(false) => {
            print_indent(indent);
            out.write_str("false\n");
        }
        Json::I64(n) => {
            print_indent(indent);
            out.write_str(n.to_string().as_slice());
            out.write_str("\n");
        }
        Json::U64(n) => {
            print_indent(indent);
            out.write_str(n.to_string().as_slice());
            out.write_str("\n");
        }
        Json::F64(n) => {
            print_indent(indent);
            out.write_str(n.to_string().as_slice());
            out.write_str("\n");
        }
    }
}

fn main() {

    let args = os::args().into_iter().collect::<Vec<String> >();
    let opts = [ optflag("j", "", "dump output in JSON, not sexp") ];
    let matches = match getopts(args.tail(), &opts) {
        Ok(m) => { m }
        Err(f) => { panic!(f) }
    };
    let dump_json = matches.opt_present("j");

    match io::stdin().read_to_string() {
        Ok(text) => {
            let opt = config::basic_options();
            let sess = session::build_session(opt, None,
                                              registry::Registry::new(&rustc::DIAGNOSTICS));
            let cfg = config::build_configuration(&sess);
            let input = Input::Str(text);
            let cr = driver::phase_1_parse_input(&sess, cfg, &input);

            // JSON-ify, meaning "encode then re-parse as just json", ugh.
            let ast_str = json::encode(&cr.module);
            let chars = ast_str.as_slice().chars().collect::<Vec<char> >();
            let mut b = json::Builder::new(chars.into_iter());
            let mut j = b.build().ok().unwrap();

            filter_json(&mut j);
            if dump_json {
                println!("{}", j.pretty());
            } else {
                print_sexp(0, &j);
            }
        },
        Err(ioerr) => {
            panic!("I/O Error: {}", ioerr.desc);
        }
    }
}
