use std::collections::HashMap;
use std::collections::HashSet;

pub fn main() {
    println!("Day11");
}

fn parse(s: &str) -> HashMap<String, Vec<String>> {
    let mut ret: HashMap<String, Vec<String>> = HashMap::new();

    let r = s.replace(":", "");

    for l in r.lines() {
        let mut entries = l.split_whitespace();
        let from: String = entries.next().unwrap().to_string();

        if !ret.contains_key(&from) {
            ret.insert(from.clone(), vec![]);
        }

        for s in entries {
            ret.get_mut(&from).unwrap().push(s.to_string());
        }
    }

    ret
}

fn solve1(paths: &HashMap<String, Vec<String>>, visited: HashSet<String>, at: &String) -> usize {
    if at == "out" {
        return 1;
    }

    let mut cnt = 0;
    for opt in paths.get(at).unwrap() {
        if !visited.contains(opt) {
            let mut h = visited.clone();
            h.insert(opt.clone());
            cnt += solve1(paths, h, opt);
        }
    }
    cnt
}

fn solve2(
    paths: &HashMap<String, Vec<String>>,
    visited: HashSet<String>,
    at: &str,
    end: &str,
    stop_after: &Vec<&str>,
) -> usize {
    if at == end {
        return 1;
    }
    if stop_after.contains(&at) {
        return 0;
    }

    let mut cnt = 0;
    for opt in paths.get(at).unwrap_or(&vec![]) {
        if !visited.contains(opt) {
            let mut h = visited.clone();
            h.insert(opt.clone());
            cnt += solve2(paths, h, opt, end, stop_after);
        }
    }
    cnt
}

fn problem1(s: &str) -> usize {
    let paths = parse(s);
    solve1(&paths, HashSet::new(), &"you".to_string())
}

fn problem2(s: &str) -> usize {
    let paths = parse(s);
    let layers = vec![
        vec!["wpq", "fnb", "zvu", "ctz", "jnl"], //
        vec!["fft"],
        vec!["cjl", "sug", "fmn", "gzw"],        //
        vec!["hdz", "kkt", "ovu", "irr", "ybz"], //
        vec!["gzx", "fkn", "jhf", "lzk", "ybz"], //
        vec!["dac"],
        vec!["hoo", "jbz", "fhk", "you"], //
        vec!["out"],
    ];

    visit(&paths, &layers, 0, "svr")
}

fn visit(
    paths: &HashMap<String, Vec<String>>,
    all: &Vec<Vec<&str>>,
    next_layer_id: usize,
    where_am_i: &str,
) -> usize {
    let mut ret = 0;
    for next in &all[next_layer_id] {
        let stop_at_id = if next == &"fft" || next == &"dac" {
            next_layer_id + 1
        } else {
            next_layer_id
        };
        //Could cache from, to here, but it seems fast enough
        let mut cnt = solve2(paths, HashSet::new(), where_am_i, next, &all[stop_at_id]);
        if cnt == 0 {
            continue;
        }
        if next_layer_id + 1 < all.len() {
            cnt = cnt * visit(paths, all, next_layer_id + 1, next);
        }
        ret += cnt;
    }

    ret
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_problems() {
        assert_eq!(5, problem1(include_str!("test_puzzle.txt")));
        assert_eq!(613, problem1(include_str!("puzzle.txt")));

        //assert_eq!(2, problem2(include_str!("test_puzzle2.txt")));
        assert_eq!(0, problem2(include_str!("puzzle.txt")));
    }
}
