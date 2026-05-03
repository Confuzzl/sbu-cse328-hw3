$f(\omega ,m)=\text{sgn}(\sin\omega)\left|\sin\omega\right|^{m}$\
$g(\omega ,m)=\text{sgn}(\cos\omega)\left|\cos\omega\right|^{m}$

$-\frac{\pi}{2} \leq v \leq +\frac{\pi}{2}$\
$-\pi \leq u \leq +\pi$

- $x(u,v)=Ag(v,\frac{2}{r})g(u,\frac{2}{r})$\
$\rho=\frac{2}{r}$\
$\begin{aligned}
\frac{\delta}{\delta u} &= Ag\left(v,\rho\right)(\text{sgn}(\cos u)\left|\cos u\right|^\rho)'\\
&= Ag\left(v,\rho\right)\begin{cases}
    0 & \cos u = 0\\
    -\rho(\sin u)(\cos u)^{\rho - 1} & \cos u > 0\\
    -\rho(\sin u)(-\cos u)^{\rho - 1} & \cos u < 0\\
\end{cases}\\
&= Ag\left(v,\rho\right)\cdot-\rho(\sin u)|\cos u|^{\rho - 1}
\end{aligned}$\
$\begin{aligned}
\frac{\delta}{\delta v} &= Ag\left(u,\rho\right)(\text{sgn}(\cos v)\left|\cos v\right|^\rho)'\\
&= Ag\left(u,\rho\right)\cdot-\rho(\sin v)|\cos v|^{\rho - 1}
\end{aligned}$

- $y(u,v)=Bg(v,\frac{2}{s})f(u,\frac{2}{s})$\
$\sigma=\frac{2}{s}$\
$\begin{aligned}
\frac{\delta}{\delta u} &= Bg\left(v,\sigma\right)(\text{sgn}(\sin u)\left|\sin u\right|^\sigma)'\\
&= Bg\left(v,\sigma\right)\begin{cases}
    0 & \sin u = 0\\
    \sigma(\cos u)(\sin u)^{\sigma - 1} & \sin u > 0\\
    --\sigma(\cos u)(-\sin u)^{\sigma - 1} & \sin u < 0\\
\end{cases}\\
&= Bg\left(v,\sigma\right)\cdot\sigma(\cos u)|\sin u|^{\rho - 1}
\end{aligned}$\
$\begin{aligned}
\frac{\delta}{\delta v} &= Bf\left(u,\sigma\right)(\text{sgn}(\cos v)\left|\cos v\right|^\sigma)'\\
&= Bf\left(u,\sigma\right)\cdot-\sigma(\sin v)|\cos v|^{\sigma - 1}
\end{aligned}$

- $z(u,v)=Cf(v,\frac{2}{t})$\
$\tau=\frac{2}{t}$\
$\begin{aligned}
\frac{\delta}{\delta u} &= 0
\end{aligned}$\
$\begin{aligned}
\frac{\delta}{\delta v} &= C(\text{sgn}(\sin v)|\sin v|^\tau)'\\
&= C\cdot\tau(\cos v)|\sin v|^{\tau - 1}
\end{aligned}$