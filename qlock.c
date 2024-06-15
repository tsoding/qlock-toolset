char *s = "?";
x, y, d[8], i, dx;
f[] = {31599,19812,14479,31207,23524,29411,29679,30866,31727,31719,1040};
char *so, *si;
p(ch)
{
    i = x/2/(3 + 2);
    dx = x/2%(3 + 2);
    if (i < 8 && (y - 4)/2 < 5 && dx < 3 && (f[d[i]]>>((5 - (y - 4)/2 - 1)*3 + dx))&1)
        printf("\033[1;41;30m%c\033[0m", ch);
    else
        printf("%c", ch);
    if (ch == '\n') {
        y += 1;
        x = 0;
    } else
        x += 1;
}
gd()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    d[0] = tm->tm_hour/10;
    d[1] = tm->tm_hour%10;
    d[2] = 10;
    d[3] = tm->tm_min/10;
    d[4] = tm->tm_min%10;
    d[5] = 10;
    d[6] = tm->tm_sec/10;
    d[7] = tm->tm_sec%10;
}

main()
{
    for (gd();;printf("\n\033[%dA\033[%dD", y + 1, x), sleep(1), gd())
        for (so = s, x = 0, y = 0; *so; so++)
            if (*so == 63)
                for (si = s; *si; si++)
                    switch (*si) {
                        case '\n': p('\\'); p('n'); p('"'); p('\n'); p('"'); break;
                        case '"':  p('\\'); p('\"'); break;
                        case '\\': p('\\'); p('\\'); break;
                        default: p(*si);
                    }
            else p(*so);
}
