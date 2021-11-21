package com.slipknosha;

import com.slipknosha.count.Counter;

public class Main {

    public static void printAuthorInformation() {
        System.out.print("\nAuthor: Vladyslav Bobrykov\n" +
                "3 course of Computer Science, group 32z");
    }

    public static void main(String[] args) {
        Counter counter;
        if (args.length > 0) {
            counter = new Counter(args[0]);
        } else {
            counter = new Counter();
        }
        counter.printCoding();
        counter.printCode();
        printAuthorInformation();
    }
}