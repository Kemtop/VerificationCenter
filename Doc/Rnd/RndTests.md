<h2>Результаты анализа ГПСЧ методом Монте-Карло.</h2>
<p>
В описании приведены результаты тестов аппаратного ГПСЧ для разных вариантов схемо техники аналоговой части, на начальной стадии разработки. 
</p>
<p>
Приведен ряд характеристик в зависимости от установленного порога компараторов. В конце таблицы приводятся характеристики при использовании многоканальных источников энтропии.
</p>
<p>
Где: q-отношение поподания точек в дугу к общему количесвтву, не критично.
pi-вычисленное число пи и должно быть похоже на реальное пи.
e-отношение вычисленного числа пи к реальному в процентах, до 1%. У криптостойкого .Net генератора 0,93-0,07
</p>
<p>
Используя текущие данные вы можете разработать свой источник энтропии. Основная трудность - подбор уровня напряжений для работы компараторов.
В приведенном исходном коде ПО не умеет автоматически вычислять качество выходной последовательности и корректировать уровни напряжений. Это следует иметь ввиду при использовании приведенной схемо техники и исходных кодов.
</p>
<table border="1" cellpadding="0" cellspacing="0" width="1025" style="border-collapse:
 collapse;table-layout:fixed;width:770pt">

 <tr height="31" style="mso-height-source:userset;height:23.25pt">
  <td height="31" class="xl65" style="height:23.25pt">Описание источника</td>
  <td class="xl66" style="border-left:none">q</td>
  <td class="xl65" style="border-left:none">pi</td>
  <td class="xl65" style="border-left:none">e,%</td>
  <td class="xl65" style="border-left:none">Бит в числе координат</td>
 </tr>
 <tr height="59" style="mso-height-source:userset;height:44.25pt">
  <td height="59" class="xl67" width="338" style="height:44.25pt;width:254pt">Файл размером 169 984 байт сгенерированный средствами C#.</td>
  <td class="xl68" align="right">0,7839796</td>
  <td class="xl68" align="right">3,1359186</td>
  <td class="xl68" align="right">0,18608</td>
  <td class="xl69" align="right">32</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,783915</td>
  <td class="xl71" align="right">3,13566</td>
  <td class="xl71" align="right">0,188746</td>
  <td class="xl72" align="right">40</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,7853865</td>
  <td class="xl71" align="right">3,14154606</td>
  <td class="xl71" align="right">0,0014829</td>
  <td class="xl72" align="right">48</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,784119</td>
  <td class="xl71" align="right">3,136479</td>
  <td class="xl71" align="right">0,16275</td>
  <td class="xl72" align="right">56</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,79273</td>
  <td class="xl71" align="right">3,170933</td>
  <td class="xl71" align="right">0,93395</td>
  <td class="xl72" align="right">64</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,78775</td>
  <td class="xl71" align="right">3,151006</td>
  <td class="xl71" align="right">0,299636</td>
  <td class="xl72" align="right">24</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,78487</td>
  <td class="xl71" align="right">3,13949</td>
  <td class="xl71" align="right">0,066755</td>
  <td class="xl72" align="right">16</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl73" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl74" align="right">0,78867</td>
  <td class="xl74" align="right">3,15469</td>
  <td class="xl74" align="right">0,41712</td>
  <td class="xl75" align="right">8</td>
 </tr>
 <tr height="48" style="height:36.0pt">
  <td height="48" class="xl67" width="338" style="height:36.0pt;border-top:none;
  width:254pt">Файл размером 10 485 760 байт, сгенерированный средствами C#</td>
  <td class="xl68" align="right" style="border-top:none">0,78903</td>
  <td class="xl68" align="right" style="border-top:none">3,15613</td>
  <td class="xl68" align="right" style="border-top:none">0,46293</td>
  <td class="xl69" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl76" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,7853</td>
  <td class="xl71" align="right">3,14122</td>
  <td class="xl71" align="right">0,011664</td>
  <td class="xl72" align="right">16</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl76" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,78498</td>
  <td class="xl71" align="right">3,13993</td>
  <td class="xl71" align="right">0,05283</td>
  <td class="xl72" align="right">24</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl76" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,78511</td>
  <td class="xl71" align="right">3,14045</td>
  <td class="xl71" align="right">0,036144</td>
  <td class="xl72" align="right">32</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl76" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,78501</td>
  <td class="xl71" align="right">3,14006</td>
  <td class="xl71" align="right">0,048529</td>
  <td class="xl72" align="right">40</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl76" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,7854</td>
  <td class="xl71" align="right">3,14161</td>
  <td class="xl71" align="right">0,000855</td>
  <td class="xl72" align="right">48</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl76" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,785328</td>
  <td class="xl71" align="right">3,14131</td>
  <td class="xl71" align="right">0,00885</td>
  <td class="xl72" align="right">56</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl77" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl74" align="right">0,78562</td>
  <td class="xl74" align="right">3,14251</td>
  <td class="xl74" align="right">0,0294</td>
  <td class="xl75" align="right">64</td>
 </tr>
 <tr height="69" style="mso-height-source:userset;height:51.75pt">
  <td height="69" class="xl67" width="338" style="height:51.75pt;border-top:none;
  width:254pt">Файл log_7_45v.bin 336 213 7,45 вольта схема с двумя
  компараторами. 22.07.2019</td>
  <td class="xl68" align="right" style="border-top:none">0,79455</td>
  <td class="xl68" align="right" style="border-top:none">3,1782</td>
  <td class="xl68" align="right" style="border-top:none">1,16553</td>
  <td class="xl69" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl70" width="338" style="height:18.0pt;width:254pt">&nbsp;</td>
  <td class="xl71" align="right">0,79321</td>
  <td class="xl71" align="right">3,17285</td>
  <td class="xl71" align="right">0,99511</td>
  <td class="xl72" align="right">16</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl78" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,79248</td>
  <td class="xl71" align="right">3,16994</td>
  <td class="xl71" align="right">0,902558</td>
  <td class="xl72" align="right">24</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl78" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,79515</td>
  <td class="xl71" align="right">3,1806</td>
  <td class="xl71" align="right">1,2417</td>
  <td class="xl72" align="right">32</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl79" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,79316</td>
  <td class="xl71" align="right">3,17265</td>
  <td class="xl71" align="right">0,988902</td>
  <td class="xl72" align="right">40</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl79" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,79201</td>
  <td class="xl71" align="right">3,16807</td>
  <td class="xl71" align="right">0,843</td>
  <td class="xl72" align="right">48</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl79" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,7911</td>
  <td class="xl71" align="right">3,16452</td>
  <td class="xl71" align="right">0,72986</td>
  <td class="xl72" align="right">56</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl80" style="height:18.0pt">&nbsp;</td>
  <td class="xl74" align="right">0,79407</td>
  <td class="xl74" align="right">3,17631</td>
  <td class="xl74" align="right">1,105387</td>
  <td class="xl75" align="right">64</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl81" style="height:18.0pt;border-top:none">Файл
  log_7_47v.bin<span style="mso-spacerun:yes">&nbsp; </span>317 586</td>
  <td class="xl68" align="right" style="border-top:none">0,79017</td>
  <td class="xl68" align="right" style="border-top:none">3,16069</td>
  <td class="xl68" align="right" style="border-top:none">0,607998</td>
  <td class="xl69" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl79" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,78602</td>
  <td class="xl71" align="right">3,14408</td>
  <td class="xl71" align="right">0,079424</td>
  <td class="xl72" align="right">16</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl79" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,7872</td>
  <td class="xl71" align="right">3,149005</td>
  <td class="xl71" align="right">0,235952</td>
  <td class="xl72" align="right">24</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,7888</td>
  <td class="xl71" align="right">3,15522</td>
  <td class="xl71" align="right">0,43383</td>
  <td class="xl72" align="right">32</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,78307</td>
  <td class="xl71" align="right">3,132313</td>
  <td class="xl71" align="right">0,2953</td>
  <td class="xl72" align="right">40</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,789004</td>
  <td class="xl71" align="right">3,15601</td>
  <td class="xl71" align="right">0,459153</td>
  <td class="xl72" align="right">48</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,78416</td>
  <td class="xl71" align="right">3,13666</td>
  <td class="xl71" align="right">0,157</td>
  <td class="xl72" align="right">56</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl83" style="height:18.0pt">&nbsp;</td>
  <td class="xl74" align="right">0,78573</td>
  <td class="xl74" align="right">3,14292</td>
  <td class="xl74" align="right">0,0425</td>
  <td class="xl75" align="right">64</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl84" style="height:18.0pt;border-top:none">log_7_48v.bin<span style="mso-spacerun:yes">&nbsp; </span>490 335</td>
  <td class="xl68" align="right" style="border-top:none">0,79151</td>
  <td class="xl68" align="right" style="border-top:none">3,16607</td>
  <td class="xl68" align="right" style="border-top:none">0,779</td>
  <td class="xl69" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,78821</td>
  <td class="xl71" align="right">3,15286</td>
  <td class="xl71" align="right">0,358</td>
  <td class="xl72" align="right">16</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,78774</td>
  <td class="xl71" align="right">3,15097</td>
  <td class="xl71" align="right">0,2986</td>
  <td class="xl72" align="right">24</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,788</td>
  <td class="xl71" align="right">3,15276</td>
  <td class="xl71" align="right">0,355</td>
  <td class="xl72" align="right">32</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,787</td>
  <td class="xl71" align="right">3,15</td>
  <td class="xl71" align="right">0,31828</td>
  <td class="xl72" align="right">40</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,786</td>
  <td class="xl71" align="right">3,14657</td>
  <td class="xl71" align="right">0,1584</td>
  <td class="xl72" align="right">48</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,786</td>
  <td class="xl71" align="right">3,146</td>
  <td class="xl71" align="right">0,1708</td>
  <td class="xl72" align="right">56</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl83" style="height:18.0pt">&nbsp;</td>
  <td class="xl74" align="right">0,78655</td>
  <td class="xl74" align="right">3,14622</td>
  <td class="xl74" align="right">0,1473</td>
  <td class="xl75" align="right">64</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl84" style="height:18.0pt;border-top:none">log_7_40v.bin
  423899</td>
  <td class="xl68" align="right" style="border-top:none">0,826</td>
  <td class="xl68" align="right" style="border-top:none">3,3</td>
  <td class="xl68" align="right" style="border-top:none">5,29</td>
  <td class="xl69" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,8259</td>
  <td class="xl71" align="right">3,3</td>
  <td class="xl71" align="right">5,16</td>
  <td class="xl72" align="right">16</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,82</td>
  <td class="xl71" align="right">3,29</td>
  <td class="xl71" align="right">4,9</td>
  <td class="xl72" align="right">24</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,823</td>
  <td class="xl71" align="right">3,29</td>
  <td class="xl71" align="right">4,85</td>
  <td class="xl72" align="right">32</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,82</td>
  <td class="xl71" align="right">3,29</td>
  <td class="xl71" align="right">4,89</td>
  <td class="xl72" align="right">40</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,82</td>
  <td class="xl71" align="right">3,3</td>
  <td class="xl71" align="right">5,15</td>
  <td class="xl72" align="right">48</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl82" style="height:18.0pt">&nbsp;</td>
  <td class="xl71" align="right">0,82</td>
  <td class="xl71" align="right">3,3</td>
  <td class="xl71" align="right">5,17</td>
  <td class="xl72" align="right">56</td>
 </tr>
 <tr height="24" style="height:18.0pt">
  <td height="24" class="xl83" style="height:18.0pt">&nbsp;</td>
  <td class="xl74" align="right">0,82</td>
  <td class="xl74" align="right">3,29</td>
  <td class="xl74" align="right">4,98</td>
  <td class="xl75" align="right">64</td>
 </tr>
 <tr height="58" style="height:43.75pt">
  <td height="58" class="xl93" width="338" style="height:43.75pt;border-top:none;
  width:254pt">log_8_01v.bin 81175<br>
    Изменен ГПСЧ<br>
    (развернута амплитуда пиков)</td>
  <td class="xl85" align="right" style="border-top:none">0,768916155</td>
  <td class="xl85" align="right" style="border-top:none">3,075664622</td>
  <td class="xl86" align="right" style="border-top:none">2,10</td>
  <td class="xl90" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,766963978</td>
  <td align="right">3,067855911</td>
  <td class="xl87" align="right">2,35</td>
  <td class="xl91" align="right">16</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,768381628</td>
  <td align="right">3,073526513</td>
  <td class="xl87" align="right">2,17</td>
  <td class="xl91" align="right">32</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,771929825</td>
  <td align="right">3,087719298</td>
  <td class="xl87" align="right">1,71</td>
  <td class="xl91" align="right">64</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,755914826</td>
  <td align="right">3,023659306</td>
  <td class="xl87" align="right">3,75</td>
  <td class="xl91" align="right">128</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl83" style="height:14.6pt">&nbsp;</td>
  <td class="xl88" align="right">0,769716088</td>
  <td class="xl88" align="right">3,078864353</td>
  <td class="xl89" align="right">2,00</td>
  <td class="xl92" align="right">256</td>
 </tr>
 <tr height="58" style="height:43.75pt">
  <td height="58" class="xl93" width="338" style="height:43.75pt;border-top:none;
  width:254pt">log_8_02v.log 91400<br>
    <span style="mso-spacerun:yes">&nbsp;</span>Уменьшена емкость <br>
    между каскадами повторителя и усилителя</td>
  <td class="xl85" align="right" style="border-top:none">0,755317287</td>
  <td class="xl85" align="right" style="border-top:none">3,021269147</td>
  <td class="xl86" align="right" style="border-top:none">3,83</td>
  <td class="xl90" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,747396061</td>
  <td align="right">2,989584245</td>
  <td class="xl87" align="right">4,84</td>
  <td class="xl91" align="right">16</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,744595186</td>
  <td align="right">2,978380744</td>
  <td class="xl87" align="right">5,20</td>
  <td class="xl91" align="right">32</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,742121849</td>
  <td align="right">2,968487395</td>
  <td class="xl87" align="right">5,51</td>
  <td class="xl91" align="right">64</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,738795518</td>
  <td align="right">2,955182073</td>
  <td class="xl87" align="right">5,93</td>
  <td class="xl91" align="right">128</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl83" style="height:14.6pt">&nbsp;</td>
  <td class="xl88" align="right">0,721988796</td>
  <td class="xl88" align="right">2,887955182</td>
  <td class="xl89" align="right">8,07</td>
  <td class="xl92" align="right">256</td>
 </tr>
 <tr height="58" style="height:43.75pt">
  <td height="58" class="xl93" width="338" style="height:43.75pt;border-top:none;
  width:254pt">log_05.bin Vref 429276<br>
    <span style="mso-spacerun:yes">&nbsp;</span>Использованы 4 источника
  энтропии.<br>
    2 из них инвертированы (1-й и 3-й)</td>
  <td class="xl85" align="right" style="border-top:none">0,789342987</td>
  <td class="xl85" align="right" style="border-top:none">3,157371947</td>
  <td class="xl86" align="right" style="border-top:none">0,50</td>
  <td class="xl90" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,783533205</td>
  <td align="right">3,134132819</td>
  <td class="xl87" align="right">0,24</td>
  <td class="xl91" align="right">16</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,785180492</td>
  <td align="right">3,140721966</td>
  <td class="xl87" align="right">0,03</td>
  <td class="xl91" align="right">32</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,78597786</td>
  <td align="right">3,143911439</td>
  <td class="xl87" align="right">0,07</td>
  <td class="xl91" align="right">64</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,783137021</td>
  <td align="right">3,132548084</td>
  <td class="xl87" align="right">0,29</td>
  <td class="xl91" align="right">128</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl83" style="height:14.6pt">&nbsp;</td>
  <td class="xl88" align="right">0,782018786</td>
  <td class="xl88" align="right">3,128075145</td>
  <td class="xl89" align="right">0,43</td>
  <td class="xl92" align="right">256</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl84" style="height:14.6pt;border-top:none">log_06.bin 2
  428 296</td>
  <td class="xl85" align="right" style="border-top:none">0,785854772</td>
  <td class="xl85" align="right" style="border-top:none">3,143419089</td>
  <td class="xl86" align="right" style="border-top:none">0,06</td>
  <td class="xl90" align="right" style="border-top:none">8</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,781812431</td>
  <td align="right">3,127249726</td>
  <td class="xl87" align="right">0,46</td>
  <td class="xl91" align="right">16</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,781216129</td>
  <td align="right">3,124864514</td>
  <td class="xl87" align="right">0,53</td>
  <td class="xl91" align="right">32</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,783340362</td>
  <td align="right">3,133361446</td>
  <td class="xl87" align="right">0,26</td>
  <td class="xl91" align="right">64</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl82" style="height:14.6pt">&nbsp;</td>
  <td align="right">0,782839597</td>
  <td align="right">3,131358389</td>
  <td class="xl87" align="right">0,33</td>
  <td class="xl91" align="right">128</td>
 </tr>
 <tr height="19" style="height:14.6pt">
  <td height="19" class="xl83" style="height:14.6pt">&nbsp;</td>
  <td class="xl88" align="right">0,784908545</td>
  <td class="xl88" align="right">3,139634178</td>
  <td class="xl89" align="right">0,06</td>
  <td class="xl92" align="right">256</td>
 </tr>
 <!--[if supportMisalignedColumns]-->
 <tr height="0" style="display:none">
  <td width="338" style="width:254pt"></td>
  <td width="171" style="width:128pt"></td>
  <td width="144" style="width:108pt"></td>
  <td width="163" style="width:123pt"></td>
  <td width="209" style="width:157pt"></td>
 </tr>
 <!--[endif]-->
</tbody></table>