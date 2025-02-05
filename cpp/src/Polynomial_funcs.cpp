#include "../header.hpp"

void KNOT::Polynomial::trim_zero(void){
    int i;
    vector<int> newcoef;
    int l, r;
    l = 0;
    while (l < digmax - digmin + 1 && coef[l] == 0){
        l++;
    }
    r = digmax - digmin;
    while (r > 0 && coef[r] == 0){
        r--;
    }
    i = l;
    while (i <= r){
        newcoef.push_back(coef[i]);
        i++;
    }
    int old_digmin = digmin;
    digmin = l + old_digmin;
    digmax = r + old_digmin;
    coef = newcoef;
}

KNOT::Polynomial KNOT::Polynomial::operator+(const Polynomial &a){
    Polynomial res;
    int i;
    res.digmin = min(this->digmin, a.digmin);
    res.digmax = max(this->digmax, a.digmax);
    res.coef.resize(res.digmax - res.digmin + 1);
    rep0(i, this->digmax - this->digmin + 1){
        res.coef[i - res.digmin + this->digmin] += this->coef[i];
    }
    rep0(i, a.digmax - a.digmin + 1){
        res.coef[i - res.digmin + a.digmin] += a.coef[i];
    }
    res.trim_zero();
    return res;
}

KNOT::Polynomial KNOT::Polynomial::operator-(const Polynomial &a){
    Polynomial res;
    int i;
    res.digmin = min(this->digmin, a.digmin);
    res.digmax = max(this->digmax, a.digmax);
    res.coef.resize(res.digmax - res.digmin + 1);
    rep0(i, this->digmax - this->digmin + 1){
        res.coef[i - res.digmin + this->digmin] += this->coef[i];
    }
    rep0(i, a.digmax - a.digmin + 1){
        res.coef[i - res.digmin + a.digmin] -= a.coef[i];
    }
    res.trim_zero();
    return res;
}

KNOT::Polynomial KNOT::Polynomial::operator*(const Polynomial &a){
    Polynomial res;
    int i, j;
    res.digmin = this->digmin + a.digmin;
    res.digmax = this->digmax + a.digmax;
    res.coef.resize(res.digmax - res.digmin + 1);
    rep0(i, this->digmax - this->digmin + 1){
        rep0(j, a.digmax - a.digmin + 1){
            res.coef[i + j] += this->coef[i] * a.coef[j];
        }
    }
    res.trim_zero();
    return res;
}

KNOT::Polynomial KNOT::Polynomial::operator/(const Polynomial &a){
    Polynomial res;
    vector<int> this_vec = this->coef;
    int i, j;
    res.digmin = this->digmin - a.digmin;
    res.digmax = this->digmax - a.digmax;
    i = 0;
    while (i < res.digmax - res.digmin + 1){
        res.coef[i] = this_vec[this->digmax - this->digmin - i] / a.coef[a.digmax - a.digmin];
        rep0(j, a.digmax - a.digmin + 1){
            this_vec[this->digmax - this->digmin - i - j] -= a.coef[a.digmax - a.digmin - j] * res.coef[i];
        }
        i++;
    }
    res.trim_zero();
    return res;
}

