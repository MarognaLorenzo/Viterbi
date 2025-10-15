
# Viterbi Example (C++)

This small project contains a simple, self-contained implementation of the Viterbi algorithm in C++ (`viterby.cpp`). It demonstrates how to compute the most likely sequence of hidden states (tags) for a sequence of observed tokens (words) using dynamic programming.

## 1. Description of the code

The repository contains a single C++ source file, `viterby.cpp`, which implements a minimal Viterbi tagger. The program defines a `Node` structure used to store per-word/per-tag scores and back-pointers, and a `Viterbi` class that builds a layered graph of nodes (one layer per input word) and performs dynamic programming to compute the highest-scoring tag sequence.

The example in `main()` uses small in-memory tables for:
- emission probabilities (score of word given a tag),
- transition probabilities (score of moving from one tag to the next), and
- initialization probabilities (score of starting in a particular tag).

The output prints debug information about layer scores and the chosen best path, and finally prints the predicted tag sequence for the input sentence.

## 2. Algorithm (theory and use-cases)

Viterbi algorithm summary:

- Problem: given a sequence of observations (e.g., words) and a Hidden Markov Model (HMM) defined by initial, transition and emission probabilities, find the most probable sequence of hidden states (e.g., part-of-speech tags) that could have produced the observations.
- Approach: dynamic programming. For every position t in the sequence and every possible state s, Viterbi stores the highest probability (score) of any path that ends in state s at position t. These values are computed iteratively from t = 0 to T-1 using the recurrence:

	v_t(s) = emission(s, obs_t) * max_{s'} ( v_{t-1}(s') * transition(s', s) )

	where v_0(s) = emission(s, obs_0) * initialization(s).

- To recover the most probable state sequence, the algorithm keeps back-pointers: for each (t, s) it stores which previous state s' gave the maximum. After filling the DP table, it selects the final state with the highest v_T(s) and follows back-pointers to reconstruct the full path.

Use-cases:
- Part-of-speech tagging in Natural Language Processing.
- Any HMM decoding problem: biological sequence labeling, speech recognition, and other sequence labeling tasks.

Notes on scores: the example stores scores as floats and uses multiplication. In real systems probabilities are often small; numerical stability is improved by working in log-space (sums of log-probabilities instead of products of probabilities).

## 3. Instructions for usage

Prerequisites:
- A C++ compiler (g++/clang++) supporting C++11 or later. On macOS you can use the system `clang++` or install `g++` via Homebrew.

Compile and run (macOS, zsh):

```bash
# from the project root (where `viterby.cpp` is located)
g++ -std=c++11 viterby.cpp -o viterby
./viterby
```

What to expect:
- The program simply prints the chosen tag sequence for the sample sentence.

Customizing the example:
- Edit `main()` in `viterby.cpp` to change `sentence`, tag list, and the `emission`, `transition`, and `initialization` tables. Recompile to run with new data.


License: this example is provided as-is for educational purposes.

