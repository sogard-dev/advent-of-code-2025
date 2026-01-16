pub fn main() {
    println!("Day10");
    println!("Summed result: {}", problem2(include_str!("puzzle.txt")));
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
    let y = m.joltage.clone();
    let x: Vec<Vec<usize>> = y
        .iter()
        .enumerate()
        .map(|(v, _)| {
            m.buttons
                .iter()
                .map(|b| if b.contains(&v) { 1 } else { 0 })
                .collect()
        })
        .collect();

    for i in 0..x.len() {
        println!("{} = {:?}", y[i], x[i]);
    }

    let mut best_presses = isize::MAX as usize;

    return recsolve(&y, &x, vec![usize::MAX; m.buttons.len()], &mut best_presses)
        .iter()
        .sum();
}
//34 = [1, 1, 1, 0, 0, 0, 0]
//18 = [0, 1, 0, 1, 1, 0, 1]
//43 = [1, 0, 1, 0, 0, 1, 1]
//15 = [1, 0, 0, 1, 0, 0, 0]
//16 = [0, 1, 0, 1, 0, 0, 1]

fn calculate_remaining(y: &Vec<usize>, x: &Vec<Vec<usize>>, a: &Vec<usize>) -> Vec<isize> {
    x.iter()
        .map(|row| {
            row.iter()
                .enumerate()
                .map(|(b, v)| if a[b] == usize::MAX { 0 } else { v * a[b] })
                .sum::<usize>()
        })
        .enumerate()
        .map(|(i, v)| y[i] as isize - v as isize)
        .collect()
}

fn recsolve(
    y: &Vec<usize>,
    x: &Vec<Vec<usize>>,
    a: Vec<usize>,
    best_presses: &mut usize,
) -> Vec<usize> {
    let current_presses: Vec<usize> = a
        .iter()
        .map(|&v| if v == usize::MAX { 0 } else { v })
        .collect();

    let res: Vec<usize> = x
        .iter()
        .map(|row| {
            row.iter()
                .enumerate()
                .map(|(b, v)| current_presses[b] * v)
                .sum()
        })
        .collect();

    //println!("Presses: {:?}", a);

    let presses = current_presses.iter().sum::<usize>();
    if &res == y {
        println!(
            "  Found solution: {:?}, presses {}",
            current_presses, presses
        );
        if presses < *best_presses {
            *best_presses = presses;
        }
        return current_presses;
    }

    let mut best = vec![usize::MAX];

    //Find list with least unknowns
    let row_opt = x
        .iter()
        .enumerate()
        .map(|(i, row)| {
            (
                i,
                row.iter()
                    .enumerate()
                    .map(|(i, v)| if *v > 0 && a[i] == usize::MAX { 1 } else { 0 })
                    .sum::<usize>(),
            )
        })
        .filter(|&v| v.1 > 0)
        .min_by_key(|(_, v)| *v);

    if row_opt.is_none() {
        return best;
    }

    let row = row_opt.unwrap();
    let first_unknown_opt = x[row.0]
        .iter()
        .enumerate()
        .filter(|&(i, v)| *v > 0 && a[i] == usize::MAX)
        .next();
    if first_unknown_opt.is_none() {
        return best;
    }
    let first_unknown = first_unknown_opt.unwrap().0;
    let max_value = y[row.0] - res[row.0];

    let from = if row.1 == 1 { max_value } else { 0 };

    for v in from..=max_value {
        let mut new_presses = a.clone();
        new_presses[first_unknown] = v;

        let delta = calculate_remaining(y, x, &new_presses);
        if *delta.iter().min().unwrap() >= 0
            && *delta.iter().max().unwrap() as usize + presses + v < *best_presses
        {
            let act = recsolve(y, x, new_presses, best_presses);
            let a: usize = act.iter().sum();
            if a < best.iter().sum() {
                best = act;
            }
        }
    }

    //Iterate over options and do recursive calls. Compare return value and return smallest
    best
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
            return solve2(parse(l));
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

        assert_eq!(33, problem2(include_str!("test_puzzle.txt")));
        assert_eq!(
            145,
            problem2(
                "[...####.#.] (0,3,4,6) (0,1,3,5,7,8) (2,3,7) (0,1,2,6,7,8,9) (0,6,9) (0,3,4,5,7,8) (1) (3,8,9) (6,9) (4,5,6,7,8,9) (1,2,3,5,6,7,8,9) (0,4,5,6,8) {76,63,42,74,41,53,91,81,69,72} "
            )
        );

        //assert_eq!(0, problem2(include_str!("puzzle.txt")));
    }
}
