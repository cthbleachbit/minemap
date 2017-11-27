Map format as of 1.10.2
=======================

NBT
---

In side a `world/data/map_xxx.dat` file:

```
0a      0000
# The root tag, a 0x0a compound tag with name length 0000 i.e. no name
        0a      0004    data
                01      0005    scale        00
#               ^ byte
                01      0009    dimension    01
#                                            ^ The End dimension
                02      0006    height       0080
#               ^ short                      ^ 128
                02      0005    width        0080
                03      0007    xCenter      05f5e100
#               ^ int                        ^ 1e8
                03      0007    zCenter      05f5e100
                07      0006    colors       00004000
#               ^ byte array                 ^ byte length: 16384
                ... (tons of bytes representing pixels)
        00
#       ^ End of data compound tag
00
# End of root tag
```

Each byte C representing a pixel is cacluated from the base color B and variant (0-3) number V. When V is 0, 1, 2, 3, the color RGB is multiplied by 180, 220, 255, 135 respectively then divided by 255 and floor to an integer RGB.

`C = B * 4 + V`

The base color table
---------------

<table><tbody>
	<tr>
		<td>0</td>
		<td></td>
		<td>Transparent</td>
	</tr>
	<tr>
		<td>1</td>
		<td bgcolor="#7fb238" style="background: #7fb238"></td>
		<td>127, 178, 56</td>
	</tr>
	<tr>
		<td>2</td>
		<td bgcolor="#f7e9a3" style="background: #f7e9a3"></td>
		<td>247, 233, 163</td>
	</tr>
	<tr>
		<td>3</td>
		<td bgcolor="#c7c7c7" style="background: #c7c7c7"></td>
		<td>199, 199, 199</td>
	</tr>
	<tr>
		<td>4</td>
		<td bgcolor="#ff0000" style="background: #ff0000"></td>
		<td>255, 0, 0</td>
	</tr>
	<tr>
		<td>5</td>
		<td bgcolor="#a0a0ff" style="background: #a0a0ff"></td>
		<td>160, 160, 255</td>
	</tr>
	<tr>
		<td>6</td>
		<td bgcolor="#a7a7a7" style="background: #a7a7a7"></td>
		<td>167,167, 167</td>
	</tr>
	<tr>
		<td>7</td>
		<td bgcolor="#007c00" style="background: #007c00"></td>
		<td>0,124, 0</td>
	</tr>
	<tr>
		<td>8</td>
		<td bgcolor="#ffffff" style="background: #ffffff"></td>
		<td>255,255, 255</td>
	</tr>
	<tr>
		<td>9</td>
		<td bgcolor="#a4a8b8" style="background: #a4a8b8"></td>
		<td>164,168, 184</td>
	</tr>
	<tr>
		<td>10</td>
		<td bgcolor="#976d4d" style="background: #976d4d"></td>
		<td>151,109, 77</td>
	</tr>
	<tr>
		<td>11</td>
		<td bgcolor="#707070" style="background: #707070"></td>
		<td>112,112, 112</td>
	</tr>
	<tr>
		<td>12</td>
		<td bgcolor="#4040ff" style="background: #4040ff"></td>
		<td>64,64, 255</td>
	</tr>
	<tr>
		<td>13</td>
		<td bgcolor="#8f7748" style="background: #8f7748"></td>
		<td>143,119, 72</td>
	</tr>
	<tr>
		<td>14</td>
		<td bgcolor="#fffcf5" style="background: #fffcf5"></td>
		<td>255,252, 245</td>
	</tr>
	<tr>
		<td>15</td>
		<td bgcolor="#d87f33" style="background: #d87f33"></td>
		<td>216,127, 51</td>
	</tr>
	<tr>
		<td>16</td>
		<td bgcolor="#b24cd8" style="background: #b24cd8"></td>
		<td>178,76, 216</td>
	</tr>
	<tr>
		<td>17</td>
		<td bgcolor="#6699d8" style="background: #6699d8"></td>
		<td>102,153, 216</td>
	</tr>
	<tr>
		<td>18</td>
		<td bgcolor="#e5e533" style="background: #e5e533"></td>
		<td>229,229, 51</td>
	</tr>
	<tr>
		<td>19</td>
		<td bgcolor="#7fcc19" style="background: #7fcc19"></td>
		<td>127,204, 25</td>
	</tr>
	<tr>
		<td>20</td>
		<td bgcolor="#f27fa5" style="background: #f27fa5"></td>
		<td>242,127, 165</td>
	</tr>
	<tr>
		<td>21</td>
		<td bgcolor="#4c4c4c" style="background: #4c4c4c"></td>
		<td>76,76, 76</td>
	</tr>
	<tr>
		<td>22</td>
		<td bgcolor="#999999" style="background: #999999"></td>
		<td>153,153, 153</td>
	</tr>
	<tr>
		<td>23</td>
		<td bgcolor="#4c7f99" style="background: #4c7f99"></td>
		<td>76,127, 153</td>
	</tr>
	<tr>
		<td>24</td>
		<td bgcolor="#7f3fb2" style="background: #7f3fb2"></td>
		<td>127,63, 178</td>
	</tr>
	<tr>
		<td>25</td>
		<td bgcolor="#334cb2" style="background: #334cb2"></td>
		<td>51,76, 178</td>
	</tr>
	<tr>
		<td>26</td>
		<td bgcolor="#664c33" style="background: #664c33"></td>
		<td>102,76, 51</td>
	</tr>
	<tr>
		<td>27</td>
		<td bgcolor="#667f33" style="background: #667f33"></td>
		<td>102,127, 51</td>
	</tr>
	<tr>
		<td>28</td>
		<td bgcolor="#993333" style="background: #993333"></td>
		<td>153,51, 51</td>
	</tr>
	<tr>
		<td>29</td>
		<td bgcolor="#191919" style="background: #191919"></td>
		<td>25,25, 25</td>
	</tr>
	<tr>
		<td>30</td>
		<td bgcolor="#faee4d" style="background: #faee4d"></td>
		<td>250,238, 77</td>
	</tr>
	<tr>
		<td>31</td>
		<td bgcolor="#5cdbd5" style="background: #5cdbd5"></td>
		<td>92,219, 213</td>
	</tr>
	<tr>
		<td>32</td>
		<td bgcolor="#4a80ff" style="background: #4a80ff"></td>
		<td>74,128, 255</td>
	</tr>
	<tr>
		<td>33</td>
		<td bgcolor="#00d93a" style="background: #00d93a"></td>
		<td>0,217, 58</td>
	</tr>
	<tr>
		<td>34</td>
		<td bgcolor="#815631" style="background: #815631"></td>
		<td>129,86, 49</td>
	</tr>
	<tr>
		<td>35</td>
		<td bgcolor="#700200" style="background: #700200"></td>
		<td>112,2, 0</td>
	</tr>
</tbody></table>
