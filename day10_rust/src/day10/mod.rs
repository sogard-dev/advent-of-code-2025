use std::collections::HashMap;
use std::collections::HashSet;

pub fn main() {
    println!("Day10");
    problem2(include_str!("puzzle.txt"));
}

#[derive(Debug, PartialEq, Clone, Eq)]
struct Machine {
    buttons: Vec<Vec<usize>>,
    joltage: Vec<usize>,
    indicator: Vec<bool>,
}

fn parse_indicator(s: &str) -> Vec<bool> {
    s[1..s.len() - 1].chars().map(|c| c == '#').collect()
}

fn parse_lights(s: &str) -> Vec<usize> {
    s[1..s.len() - 1]
        .split(",")
        .map(|s| s.parse::<usize>().unwrap())
        .collect()
}

fn parse_joltage(s: &str) -> Vec<usize> {
    s[1..s.len() - 1]
        .split(",")
        .map(|s| s.parse::<usize>().unwrap())
        .collect()
}

fn parse(line: &str) -> Machine {
    let segments: Vec<&str> = line.split_whitespace().collect();
    let indicator = parse_indicator(segments[0]);
    let buttons = segments[1..segments.len() - 1]
        .iter()
        .map(|k| parse_lights(k))
        .collect();
    let joltage = parse_joltage(segments.last().unwrap());

    Machine {
        buttons,
        joltage,
        indicator,
    }
}

fn print_machine(m: &Machine) {
    println!("Printing machine");
    println!("{:#?}", m);
}

fn solve1(m: Machine) -> i32 {
    let iterations = 1 << m.buttons.len();

    let mut best = i32::MAX;
    for i in 0..iterations {
        let mut lights: Vec<bool> = vec![false; m.indicator.len()];
        let mut moves = 0;
        for (bid, b) in m.buttons.iter().enumerate() {
            if (i >> bid) & 1 == 1 {
                moves = moves + 1;
                for l in b.iter() {
                    lights[*l] = !lights[*l]
                }
            }
        }

        let mut identical = true;
        for i in 0..lights.len() {
            identical = identical && lights[i] == m.indicator[i]
        }

        if identical {
            best = i32::min(best, moves);
        }
    }

    return best;
}

fn solve2(m: Machine) -> usize {
    let mut bidx_map: HashMap<usize, Vec<usize>> = HashMap::new();
    for (i, _) in m.joltage.iter().enumerate() {
        bidx_map.insert(i, Vec::new());
    }
    for (bidx, b) in m.buttons.iter().enumerate() {
        for lidx in b {
            bidx_map.get_mut(lidx).unwrap().push(bidx);
        }
    }
    for (_, v) in bidx_map.iter_mut() {
        v.sort_by(|a, b| m.buttons[*a].len().cmp(&m.buttons[*b].len()));
    }

    let btn_cnt = m.buttons.len();
    let joltage_min = *m.joltage.iter().min().unwrap();

    println!("Need to solve: {:?}", m.joltage);
    let mut joltages = m.joltage.clone();
    let mut precompute_presses = 0;
    let joltage_to_reduce = btn_cnt;
    if joltage_to_reduce < joltage_min {
        let to_solve = m.joltage.iter().map(|_| joltage_to_reduce).collect();
        let mut precompute_best_presses = usize::MAX;
        recsolve(
            &to_solve,
            &m.buttons,
            0,
            &mut precompute_best_presses,
            &bidx_map,
            vec![0; m.buttons.len()],
            &mut HashSet::new(),
        );
        println!(
            "  Precompute: Took {} to solve {:?}",
            precompute_best_presses, to_solve
        );
        precompute_presses = precompute_best_presses;

        joltages = joltages.iter().map(|v| v - joltage_to_reduce).collect();
    }

    println!("  Solving remaining: {:?}", joltages);
    let mut best_presses = usize::MAX;
    recsolve(
        &joltages,
        &m.buttons,
        0,
        &mut best_presses,
        &bidx_map,
        vec![0; m.buttons.len()],
        &mut HashSet::new(),
    );
    precompute_presses + best_presses
}

fn recsolve(
    joltages_remaining: &Vec<usize>,
    buttons: &Vec<Vec<usize>>,
    presses: usize,
    best_press: &mut usize,
    bidx_map: &HashMap<usize, Vec<usize>>,
    btn_presses: Vec<usize>,
    visited: &mut HashSet<Vec<usize>>,
) {
    if presses >= *best_press || visited.contains(&btn_presses) {
        return;
    }

    let sum_remaining: usize = joltages_remaining.iter().sum();
    if sum_remaining == 0 {
        *best_press = usize::min(*best_press, presses);
        println!(
            "Found button combination {:?} of presses: {}",
            btn_presses, presses
        );
        return;
    }

    visited.insert(btn_presses.clone());

    let remaining = *joltages_remaining
        .iter()
        .filter(|l| **l > 0 as usize)
        .min()
        .unwrap();
    for (i, v) in joltages_remaining.iter().enumerate() {
        if *v == remaining {
            for bidx in bidx_map.get(&i).unwrap() {
                let b = &buttons[*bidx];
                let mut skip = false;
                let mut toggle = joltages_remaining.clone();
                for l in b {
                    if toggle[*l] == 0 {
                        skip = true;
                        break;
                    }
                    toggle[*l] = toggle[*l] - 1;
                }

                if !skip {
                    let mut bn = btn_presses.clone();
                    bn[*bidx] += 1;
                    recsolve(
                        &toggle,
                        buttons,
                        presses + 1,
                        best_press,
                        bidx_map,
                        bn,
                        visited,
                    );
                }
            }
            break;
        }
    }
}

fn problem1(s: &str) -> i32 {
    s.lines()
        .map(|l| {
            let res = solve1(parse(l));
            return res;
        })
        .sum()
}

fn problem2(s: &str) -> usize {
    let c = s.lines().count();
    s.lines()
        .enumerate()
        .map(|(i, l)| {
            println!("Starting on machine {} / {}", i + 1, c);
            let res = solve2(parse(l));
            println!("Result was {}", res);
            return res;
        })
        .sum()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_problems() {
        //assert_eq!(7, problem1(include_str!("test_puzzle.txt")));
        //assert_eq!(494, problem1(include_str!("puzzle.txt")));

        //assert_eq!(33, problem2(include_str!("test_puzzle.txt")));
        assert_eq!(
            49,
            problem2("[##.##] (0,2,3) (0,1,4) (0,2) (1,3,4) (1) (2) (1,2,4) {34,18,43,15,16}")
        );
        assert_eq!(
            60,
            problem2("[..#.] (1,2) (0,3) (0,2,3) (0,2) (1,2,3) {40,20,48,29}")
        );
        assert_eq!(
            40,
            problem2(
                "[.#...#..] (1,3,5,6,7) (1,3,4,5,6) (1,5) (1,7) (0,1,2,5,6,7) (0,5) (0,1,2,3,4,7) (1,3,4,5,6,7) {13,38,11,7,7,20,18,31}"
            )
        );
        assert_eq!(0, problem2(include_str!("puzzle.txt")));
    }
}
