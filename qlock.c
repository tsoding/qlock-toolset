f[] = {31599,19812,14479,31207,23524,29411,29679,30866,31727,31719,1040};
x, y, x2, y4, i, j, h, m, q, T=10;
struct tm *tm;
char *so, *si;

gt()
{
    time_t t = time(NULL);
    tm = localtime(&t);
}

int d(g)
{
    h = tm->tm_hour;
    m = tm->tm_min;
    q = tm->tm_sec;
    int d[8] = { h/T, h%T, T, m/T, m%T, T, q/T, q%T };
    return d[g];
}

p(ch)
{
    y4 = y/2 - 1;
    x2 = x/2 - 2;
    i = x2/5;
    j = x2%5;
    if (x > 2 && i < 8 && y4 < 5 && j < 3 && (f[d(i)]>>(12 - y4*3 + j)) & 1)
        z("\033[1;41;30m%c\033[0m", ch);
    else
        z("%c", ch);
    if (ch == '\n') {
        y += 1;
        x = 0;
    } else
        x += 1;
}

char *s = "?";
main()
{
    for (gt();;z("\n\033[%dA\033[%dD", y + 1, x), sleep(1), gt())
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
