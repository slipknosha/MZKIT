package com.slipknosha.count;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

public class Counter {

    private static class Letter implements Comparable<Letter> {
        final private int letter;
        private int counter = 0;
        private double p_alpha;
        private double info;
        private String code = "";

        public Letter(int letter) {
            this.letter = letter;
        }

        public void incCounter() {
            ++counter;
        }

        public int compareTo(Letter o) {
            return o.counter - this.counter;
        }

        public char getLetter() {
            return (char) letter;
        }
    }

    int alphabet_amount = 58;
    Letter[] letters = new Letter[alphabet_amount];
    int amount;

    public Counter() {
        Scanner reader = new Scanner(System.in);

        for (int i = 0; i < 26; ++i) {
            letters[i] = new Letter(i + 'a');
        }
        for (int i = 26; i < alphabet_amount; ++i) {
            letters[i] = new Letter(i + 6);
        }

        String s;
        while (!((s = reader.nextLine()).equals("0"))) {
            char[] cArr = s.toCharArray();
            for (char c : cArr) {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    if (c >= 'a')
                        letters[c - 'a'].incCounter();
                    else
                        letters[c - 'A'].incCounter();
                    ++amount;
                }
                if (c >= ' ' && c <= '?') {
                    letters[c - 6].incCounter();
                    ++amount;
                }
            }
        }

        processingData();
    }

    public Counter(String path) {

        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(new File(path)));

            for (int i = 0; i < 26; ++i) {
                letters[i] = new Letter(i + 'a');
            }
            for (int i = 26; i < alphabet_amount; ++i) {
                letters[i] = new Letter(i + 6);
            }

            int c;
            while ((c = reader.read()) != -1) {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    if (c >= 'a')
                        letters[c - 'a'].incCounter();
                    else
                        letters[c - 'A'].incCounter();
                    ++amount;
                }
                if (c >= ' ' && c <= '?') {
                    letters[c - 6].incCounter();
                    ++amount;
                }

            }

            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        processingData();
    }

    private void processingData() {
        for (Letter i : letters) {
            if (i.counter == 0) {
                --alphabet_amount;
            }
        }
        calculatePAlpha();
        sort();
        coding(0, alphabet_amount);
    }

    private void calculatePAlpha() {
        for (int i = 0; i < 58; ++i) {
            letters[i].p_alpha = (double) letters[i].counter / amount;
        }
    }

    public void printCoding() {
        System.out.print("\nCoding process:\n\n");
        int amount = letters[alphabet_amount - 1].code.length();
        for(int i = 0; i < alphabet_amount; ++i) {
            if (letters[i].code.length() > amount) {
                amount = letters[i].code.length();
            }
        }

        for (int i = 0; i < alphabet_amount; ++i) {
            System.out.print(letters[i].getLetter() + "\t| ");
            char[] buf = letters[i].code.toCharArray();
            for (char c : buf) {
                System.out.print(c + " | ");
            }
            for (int j = buf.length; j < amount; ++j) {
                System.out.print("  | ");
            }
            System.out.print(letters[i].code + "\n");
        }
    }

    public void printCode() {
        System.out.print("\nCoding Table:\n\n");
        for (int i = 0; i < alphabet_amount; ++i) {
            System.out.println((char) letters[i].letter + " - " + letters[i].code);
        }
    }

    public void sort() {
        Arrays.sort(letters);
    }

    private int divideArray(int begin, int end) {
        double sum = 0;
        for (int i = begin; i < end; ++i) {
            sum += letters[i].p_alpha;
        }
        sum /= 2;

        double curValue = 0;
        double prevValue = sum;
        int index = begin;
        for (int i = begin; i < end; ++i) {
            curValue += letters[i].p_alpha;
            if (Math.abs(curValue - sum) < prevValue) {
                index = i;
                prevValue = Math.abs(curValue - sum);
                continue;
            }
            break;
        }
        return index + 1;
    }

    private void coding(int begin, int end) {
        if (begin == end - 1) {
            return;
        }
        int middle = divideArray(begin, end);
        for (int i = begin; i < middle; ++i) {
            letters[i].code += "1";
        }
        for (int i = middle; i < end; ++i) {
            letters[i].code += "0";
        }
        coding(begin, middle);
        coding(middle, end);
    }
}

