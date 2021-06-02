#include "OrganVoice.h"
// lossy Pipe coefficients

// Auto-generated in MatLab at 30-May-2021 21:35:02 by Champ Darabundit
double OrganVoice::principalDelay[65] = {
    0.00035587773795765207,0.00033928618259121450,0.00032292432839783734,0.00030731904366941440,0.00029287279818969686,0.00027866155584400102,0.00026513746055744318,0.00025261133988854225,0.00024025721075987444,0.00022855088773593692,0.00021740199477266855,0.00020676387438070898,0.00019664579116017466,0.00018700055763241460,0.00017781925664981263,0.00016906491740890754,0.00016074572734376736,0.00015266177482632577,0.00014510728337341277,0.00013780241932014137,0.00013096985547953943,0.00012435430318167071,0.00011805021392466615,0.00011216304275441294,0.00010647883079407456,0.00010098326952617317,0.00009584147659614174,0.00009090252081199529,0.00008617895494167757,0.00008179413192413371,0.00007747664702089879,0.00007344378385941803,0.00006956717194843187,0.00006586829428845373,0.00006239276812994003,0.00005905769564074259,0.00005587338114560713,0.00005324803309851631,0.00005016376271411204,0.00004802852024755370,0.00004468678514479626,0.00004217329886527916,0.00003986531451015477,0.00003778414923779830,0.00003561689593562349,0.00003367077722762402,0.00003251648677247613,0.00003048344840810222,0.00002851707618555099,0.00002673927098521444,0.00002505400081824893,0.00002345836638658671,0.00002183703104272564,0.00002051827208150606,0.00001913276124035889,0.00001803565562137684,0.00001679130249300483,0.00001532230618605219,0.00001489126021843667,0.00001345314971894262,0.00001261767199074838,0.00001196822049857822,0.00001094710548074561,0.00001046202801214700,0.00000944274357948618};

// Auto-generated in MatLab at 30-May-2021 21:35:02 by Champ Darabundit
double OrganVoice::principalCoefs[65][5] = {
    {0.01665006949420601290, 0.00068359300356398800, -0.00450505339670235604, -1.74534389528605959896, 0.75817250438712724403},
    {0.01787197359688137005, 0.00071713635112983360, -0.00462551678145571490, -1.73468180955580408842, 0.74864540272235957641},
    {0.01918229959060257367, 0.00076124475632692275, -0.00475214125020298819, -1.72369401963051882554, 0.73888542272724533344},
    {0.02058668211373072821, 0.00081699392681699198, -0.00488315466864509395, -1.71234420858290614653, 0.72886472995480877657},
    {0.02207434880924760007, 0.00091480379219814807, -0.00503329390293250859, -1.70066070203726904531, 0.71861656073578228376},
    {0.02370605199319261031, 0.00096411814863419981, -0.00515699604020968157, -1.68852691658619513682, 0.70804009068781226244},
    {0.02541893092727977657, 0.00108510633482285603, -0.00531263619436576386, -1.67605674473678312175, 0.69724814580451999202},
    {0.02708372334715641871, 0.00151132308192232609, -0.00562369554032639708, -1.66360230873591974365, 0.68657365962467209464},
    {0.02923952004319682260, 0.00134413923449342213, -0.00562096013921664639, -1.64990063594017555282, 0.67486333507864915138},
    {0.03134581943165387630, 0.00151505109778893136, -0.00578348366168051926, -1.63614465607367631783, 0.66322204294143860537},
    {0.03354759055463486100, 0.00179425773961847720, -0.00599963364113707646, -1.62213262246193345995, 0.65147483711504972081},
    {0.03599872995442755796, 0.00194336905258849554, -0.00613310816750511738, -1.60740541058603536584, 0.63921440142554630803},
    {0.03853095157600494591, 0.00225936139848440529, -0.00635174244295513921, -1.59249380536004458975, 0.62693237589157879697},
    {0.04128673510379279876, 0.00252667535129146633, -0.00652664976737906376, -1.57694580033132969454, 0.61423256101903489501},
    {0.04419234148282678160, 0.00288592577008711356, -0.00673943334257867937, -1.56102559386952166598, 0.60136442777985688046},
    {0.04735630848432020867, 0.00318305515228377517, -0.00689397458986955661, -1.54439758623062584064, 0.58804297527736026918},
    {0.05056206389283098013, 0.00378247258253285815, -0.00722622851679464127, -1.52792333580490602429, 0.57504164376347521870},
    {0.05416033023228394166, 0.00415087531069998662, -0.00738927183252764078, -1.51034648112603631098, 0.56126841483649259068},
    {0.05783810233632148690, 0.00480382660593277853, -0.00770897314930383496, -1.49280837136502930740, 0.54774132715797974047},
    {0.06199008950806094570, 0.00516390265556378181, -0.00776840158644210785, -1.47355812078637504747, 0.53294371136355767060},
    {0.06608536297998733600, 0.00603161781029575451, -0.00822575432203003637, -1.45564266548937770729, 0.51953389195763077790},
    {0.07061821511808460961, 0.00671926750296748462, -0.00847171907619579875, -1.43615192905683275448, 0.50501769260168904996},
    {0.07552813951223995292, 0.00733371984948913164, -0.00862416450536764717, -1.41580327735404454259, 0.49004097221040598953},
    {0.08045622578876875097, 0.00841012633467020168, -0.00910569252352099595, -1.39616358752623970219, 0.47592424712615766236},
    {0.08570100463629211351, 0.00954418313976496124, -0.00958719406110239078, -1.37591707006968722027, 0.46157506378464191465},
    {0.09144143331262308272, 0.01050483051288680046, -0.00989633485183345936, -1.35455714425070095075, 0.44660707322437737110},
    {0.09749897322570331981, 0.01170042933020840059, -0.01007303784069363967, -1.33118596535992383956, 0.43031233007514191335},
    {0.10408430927608061656, 0.01247831864626339216, -0.01041407470402517209, -1.30996082591338480761, 0.41610937913170364943},
    {0.11089830477159863531, 0.01367139305946860409, -0.01073909787381848238, -1.28699955031309176690, 0.40083015027034052391},
    {0.11763736870665740530, 0.01558885918827917876, -0.01155610917154011735, -1.26469013580390443607, 0.38636025452730088370},
    {0.12552339280965488233, 0.01645484818807098695, -0.01169716719174770589, -1.24090274233510577950, 0.37118381614108392208},
    {0.13325790480507748725, 0.01822477603582817879, -0.01239796911125107086, -1.21754361132457611383, 0.35662832305423070034},
    {0.14185447359392075461, 0.01961093253636610884, -0.01253387566419771580, -1.19167087553988748638, 0.34060240600597663230},
    {0.15051474109749601871, 0.02119220000203044657, -0.01342831438299731291, -1.16920963062086702422, 0.32748825733739617139},
    {0.15969671461150178327, 0.02296071267421233991, -0.01407749974660995121, -1.14445266845495052799, 0.31303259599405469649},
    {0.16927485933184277522, 0.02479893974446347599, -0.01488038760438887241, -1.11997648876884148272, 0.29916990024075884591},
    {0.17953825544446058715, 0.02645715616636667641, -0.01543647268713411666, -1.09435892086935293577, 0.28491785979304606879},
    {0.18848632720795305273, 0.03831932628886285802, -0.01281882131312244918, -1.02724198828349821966, 0.24122882046719168470},
    {0.20027856977241942360, 0.03531462191827067915, -0.01559376603811008476, -1.02399964414424782255, 0.24399906979682783881},
    {0.21019269096071704905, 0.06351915096875998534, -0.00440857231490781459, -0.88148706756460992739, 0.15079033717917911250},
    {0.22442933140159734839, 0.03259502437292170762, -0.02039310290771031292, -1.00079153817628441203, 0.23742279104309313431},
    {0.23708929371756359838, 0.03226348365470575047, -0.02232743392902530183, -0.98299209622545580256, 0.23001743966869980795},
    {0.24981044986134587349, 0.03333059906245595255, -0.02426050792650489771, -0.96095346612039855838, 0.21983400711769546243},
    {0.26282907484996537173, 0.05060277122079301360, -0.01831552169292446214, -0.87583251534980999242, 0.17094883972764393643},
    {0.27667112813619004763, 0.04699901350989107301, -0.02251674741279620362, -0.87207336659180711980, 0.17322676082509200213},
    {0.29079033077213439773, 0.06175849388340576979, -0.01838150663198667017, -0.80246373379480273602, 0.13663105181835624724},
    {0.30352171095795937861, 0.43180373476438166369, 0.12822772921380562461, 0.42984004495724309480, -0.56628687002109634463},
    {0.31897750661726215204, 0.44579124073751347224, 0.12866691375254149521, 0.43381702938393168489, -0.54038136827661453765},
    {0.33522284938994378889, 0.46950472866279902018, 0.13423916255862991531, 0.46765580127145023326, -0.52868906066007748112},
    {0.35191190434389796327, 0.33098356576063192591, 0.07410486402505207792, 0.03901997241048005272, -0.28201963828089809949},
    {0.36854844132036945759, 0.45935520121328798915, 0.11844739548670561724, 0.37350220725476079853, -0.42715116923439766516},
    {0.38557734605745014322, 0.42707963905847473596, 0.10124466765015129044, 0.26355558618798546888, -0.34965393342190931314},
    {0.40302934616099073128, 0.54808753445872193666, 0.14530946035746294953, 0.54347398654905576532, -0.44704764557188036989},
    {0.42063723125705065886, 0.51521180821380463399, 0.12448415871927667742, 0.43862204364711171456, -0.37828884545697966102},
    {0.43735254444498883331, 0.58493065087041062888, 0.14768789621383188182, 0.57277685977895342884, -0.40280576824972208483},
    {0.45633172238512770269, 0.44226717474100640803, 0.08666103882334193209, 0.24151383833501238740, -0.25625390238553646949},
    {0.47499287531057921008, 0.51747227241350024496, 0.10912635140448848081, 0.39073186399920650791, -0.28914036487063854430},
    {0.49517390358579854714, 0.65772393539040263644, 0.16659795020786788422, 0.65354308980246755834, -0.33404730061839860156},
    {0.50980073412924353793, 0.31719408871975068864, 0.03023515052179437043, -0.02063144391373650591, -0.12213858271547479994},
    {0.53179194704669652261, 0.69164409740078502953, 0.16141567063404801963, 0.68550782655980446734, -0.30065611147827486782},
    {0.54773420507068204532, 0.70250430380978812206, 0.15767655602788119285, 0.68653598034288432217, -0.27862091543453321174},
    {0.56560373623185000724, 0.54753698815366069752, 0.09257626001784703684, 0.40581385499611755119, -0.20009687059275987897},
    {0.58420220379637455288, 0.74486009606543746031, 0.16205301565191140845, 0.73163292568212423550, -0.24051761016840067509},
    {0.60079888023014393905, 0.51482997924302342785, 0.07151673046627998742, 0.34710935876349935381, -0.15996376882405219377},
    {0.62089513417218489266, 0.78067616301750741847, 0.16583711482805754689, 0.76637030895479252290, -0.19896189693704285917}};

// Auto-generated in MatLab at 30-May-2021 21:37:59 by Champ Darabundit
double OrganVoice::fluteDelay[65] = {
    0.00055706990145438617,0.00053189211430131620,0.00050591722410851857,0.00048116254032970626,0.00045925577564807788,0.00043684162651580656,0.00041550432267049755,0.00039649657306258971,0.00037710770380188494,0.00035867110661886795,0.00034121366104615178,0.00032455888197458502,0.00030875958479532239,0.00029376673097878547,0.00027947006321310471,0.00026586894660898450,0.00025295716765900565,0.00024015142268301901,0.00022848379717374219,0.00021693760233658236,0.00020644281683835546,0.00019603452723561759,0.00018618127145416695,0.00017711456206318771,0.00016829411412229737,0.00015958913863480266,0.00015157054983333523,0.00014397461884930798,0.00013656226957014400,0.00012976404483756648,0.00012305826692152961,0.00011600081662670167,0.00011073745629815844,0.00010493715582029569,0.00009954580275863605,0.00009433247860775750,0.00008938642938132442,0.00008479020672978920,0.00008021708963915617,0.00007604931752209391,0.00007191055805320092,0.00006799700525037025,0.00006436665775062651,0.00006106926012399504,0.00005752225511882043,0.00005437585300024357,0.00005140203677973900,0.00004853673629145409,0.00004578152393446209,0.00004318671934541994,0.00004073026371893289,0.00003837249753054620,0.00003753695341798955,0.00003414193903080038,0.00003296422911433240,0.00003015703400485821,0.00002882224556688742,0.00002698463181169344,0.00002520829622358863,0.00002360378653339527,0.00002220832372166661,0.00002051933814608089,0.00001925098115407556,0.00001782388267180267,0.00001667765245562083};

// Auto-generated in MatLab at 30-May-2021 21:37:59 by Champ Darabundit
double OrganVoice::fluteCoefs[65][5] = {
    {0.00898140738333613221, 0.00026345387739033820, -0.00367893466738959153, -1.83038406322485602828, 0.83594998981819290673},
    {0.00964198807914066168, 0.00021148224803419668, -0.00376740146344898977, -1.82268162526991672756, 0.82876769413364259620},
    {0.01035063004968621130, 0.00016840152795163571, -0.00387404929631735787, -1.81487885055500441922, 0.82152383283632490762},
    {0.01110679776326323372, 0.00013577651914159394, -0.00399208077401591761, -1.80686641881152998401, 0.81411691231991889506},
    {0.01192336804412943811, 0.00009493619012288618, -0.00410871826096182412, -1.79854138246641914733, 0.80645096843970964784},
    {0.01280356094576603002, 0.00004623677792501538, -0.00422148980520048196, -1.78984925739442446435, 0.79847756531291502746},
    {0.01373964779845451845, 0.00001491778568085545, -0.00434866150466075869, -1.78092880248483087513, 0.79033470656430548917},
    {0.01470459535625163669, 0.00005401470583370934, -0.00451090425817613373, -1.77173694024007577497, 0.78198464604398498778},
    {0.01581652231975336914, -0.00003219735439343195, -0.00462021506447303758, -1.76217718592331440419, 0.77334129582420130244},
    {0.01698042813343812393, -0.00006841142410017313, -0.00475873438503804109, -1.75238424566261286053, 0.76453752798691276826},
    {0.01820528993737820581, -0.00006540679934221270, -0.00491000359863064031, -1.74214300028791635455, 0.75537287982732170821},
    {0.01954136270172094139, -0.00009320586376221304, -0.00505689097071379805, -1.73166962497759313599, 0.74606089084483806495},
    {0.02087865817736746232, 0.00004650208330611413, -0.00528783060894698082, -1.72101902989026966928, 0.73665635954199626401},
    {0.02244366504090247044, -0.00003077961993749013, -0.00539891524271629911, -1.70952422459848496494, 0.72653819477673364791},
    {0.02409132063015151717, -0.00005377443471847733, -0.00554566751124659172, -1.69785335166287021380, 0.71634523034705666067},
    {0.02583861710132696540, -0.00004131768887644671, -0.00571108412295879241, -1.68585093429853327684, 0.70593714958802500359},
    {0.02766311332675002246, 0.00005793753061606083, -0.00591679469794602689, -1.67349457833495041115, 0.69529883449437046572},
    {0.02962193472568591321, 0.00016156636838836769, -0.00612516516879917181, -1.66075545454650019828, 0.68441379047177530737},
    {0.03176791833240764223, 0.00019519631538061577, -0.00629300585031315144, -1.64747783858832952397, 0.67314794738580463473},
    {0.03405316435472702241, 0.00025743607018346813, -0.00647424927775561588, -1.63383368609678747418, 0.66167003724394235498},
    {0.03633402994970503369, 0.00060610787126616212, -0.00680565775282785529, -1.62011639755305658639, 0.65025087762119992973},
    {0.03901344208599349939, 0.00058245805616770840, -0.00692295969378055407, -1.60533372519438310988, 0.63800666564276375947},
    {0.04176781754631115479, 0.00074875366223043185, -0.00714033279196670152, -1.59039110208917988665, 0.62576734050575477664},
    {0.04467562366220011183, 0.00099056722984110375, -0.00740611541579700144, -1.57526654207586891410, 0.61352661755211312933},
    {0.04749142256202518869, 0.00173185593092232254, -0.00794350192205885847, -1.56025500359402213846, 0.60153478016491079838},
    {0.05089150772609925266, 0.00188108699464192689, -0.00812222788270170956, -1.54375910078611311427, 0.58840946762415258231},
    {0.05460412446707707534, 0.00191713723341995022, -0.00820896913480977271, -1.52644292189451058483, 0.57475521446019783856},
    {0.05838072968675554753, 0.00225670568748370095, -0.00847446918799275167, -1.50923257898969520596, 0.56139554517594170147},
    {0.06242557009066852791, 0.00259839292861975156, -0.00871411599128214809, -1.49137803100557086999, 0.54768787803357699140},
    {0.06634502035866218783, 0.00356824475690954539, -0.00937210805835876973, -1.47443403997352295853, 0.53497519703073592723},
    {0.07117363646682914324, 0.00359700189285803865, -0.00935684961561862594, -1.45474142594000066531, 0.52015521468406922256},
    {0.07464883867473109946, 0.00530614906341166787, -0.01283757777055246144, -1.45987796521705059938, 0.52699537518464090180},
    {0.08110719439738441061, 0.00472028565255845807, -0.00994803156043874356, -1.41567169595647901481, 0.49155114444598313472},
    {0.08640883606849218690, 0.00551092012504155772, -0.01044803414600268057, -1.39633585504105317554, 0.47780757708858423438},
    {0.09218191511467314914, 0.00618343487731491757, -0.01073420532387574060, -1.37537414032390947760, 0.46300528499202181498},
    {0.09813427741691094219, 0.00709393241515465008, -0.01128015065010566989, -1.35516195113439374964, 0.44911001031635366942},
    {0.10457773279349924511, 0.00795152200033484753, -0.01157942284803976755, -1.33296169923584328920, 0.43391153118163761082},
    {0.11062339039241747551, 0.00994239939588616173, -0.01275459817842736036, -1.31267264245277082146, 0.42048383406264711049},
    {0.11808868788394936977, 0.01039888318604573376, -0.01295349295265904414, -1.29030964929936176055, 0.40584372741669783036},
    {0.12531676849168349452, 0.01200213798141037611, -0.01365634286308415636, -1.26723685898340976408, 0.39089942259341947661},
    {0.13381165415439644217, 0.01216176763929276150, -0.01375358439896955977, -1.24448727032470829457, 0.37670710771942794715},
    {0.14214287136020692381, 0.01334242335856211659, -0.01436737057444909156, -1.22145387538090632873, 0.36257179952522627930},
    {0.15085466474576542106, 0.01462849206715265658, -0.01508598508310589684, -1.19819102537217525573, 0.34858819710198746256},
    {0.15876493896343191192, 0.01986560823454007821, -0.01549881193098716359, -1.16173907563096467932, 0.32487081089794950239},
    {0.16951482716446314436, 0.01752546589289604925, -0.01671805180778146785, -1.15057820114242970178, 0.32090044239200743448},
    {0.17970491730077323145, 0.01859908321075027376, -0.01755273459847174131, -1.12720626070695351117, 0.30795752662000530631},
    {0.18999630066489692215, 0.02089748475476072703, -0.01837277689427202140, -1.10009769179949024043, 0.29261870032487585780},
    {0.20100846825920198890, 0.02349103161755583535, -0.01862347333363306018, -1.06943568774938113641, 0.27531171429250589355},
    {0.21258447190943122185, 0.02299371774805086463, -0.02071375133101875654, -1.05413223113093135197, 0.26899666945739469925},
    {0.22475419465551735310, 0.02311821683893306931, -0.02218548772337242819, -1.03366965719443193450, 0.25935658096550995300},
    {0.23715253507678721312, 0.02421271847126223176, -0.02360974413275902448, -1.00999496231463159468, 0.24775047172992201161},
    {0.25018923685748051255, 0.02493258848242257963, -0.02497162265626198113, -0.98636155446227380406, 0.23651175714591488042},
    {0.25804369823062495160, 0.37224691140907939513, 0.11512019060924369440, 0.35335416230781696578, -0.60794336205886878588},
    {0.27674056059832202736, 0.04178265009031085359, -0.02208269669009496269, -0.88599705963491248717, 0.18243757363345047828},
    {0.28796642395243016166, 0.41066424985594590646, 0.12280626042857391289, 0.40566405402312927242, -0.58422711978617936079},
    {0.30658284992164730465, 0.02685181652286863091, -0.03197921909217390363, -0.89278923276874089243, 0.19424468012108292436},
    {0.31861479322657765278, 0.45000425086037187938, 0.13199694680447202066, 0.44900455899530722359, -0.54838856810388569851},
    {0.33514707080735856026, 0.46915754695934963614, 0.13401169835307197298, 0.46939560817042436103, -0.53107929205064419165},
    {0.35182443614708230895, 0.48892387907174261752, 0.13702892146327624867, 0.48931903190426079275, -0.51154179522215970088},
    {0.36857139065385025045, 0.46848268671496462456, 0.12212063399299467881, 0.39953688413549304226, -0.44036217277368361334},
    {0.37949171602667375325, 0.50147113734234982640, 0.12723475104570400318, 0.44267473642927829269, -0.43447713201455073762},
    {0.40342517404097338884, 0.55166307550623339129, 0.14808644533859002812, 0.55192048706503582522, -0.44874579217923898922},
    {0.41884293592531268713, 0.55256876627523687784, 0.13842504243938713593, 0.52085154204351535867, -0.41101479740357865778},
    {0.43980755881148436703, 0.59479839238168874527, 0.15489409807555501453, 0.59496197481549606412, -0.40546192554676796505},
    {0.45668940178994954504, 0.60516699926993977598, 0.15073805416115917954, 0.59089058620127887789, -0.37829613098023057161}};

// Auto-generated in MatLab at 30-May-2021 21:46:20 by Champ Darabundit
double OrganVoice::violinDelay[65] = {
    0.00025878939703048346,0.00024658252621926870,0.00023472886167987629,0.00022340485965142455,0.00021279191465658752,0.00020247006184204429,0.00019265257191461018,0.00018344361564170722,0.00017446245155908536,0.00016593994954776305,0.00015783368694644826,0.00015007264341027534,0.00014270049886408703,0.00013564994822506319,0.00012895853836167372,0.00012255547702909301,0.00011650951708503239,0.00011061812332908213,0.00010511085424334341,0.00009975670581619966,0.00009474687328973693,0.00008997342372105617,0.00008532444721576424,0.00008103639447105340,0.00007686269995368071,0.00007286244342523324,0.00006914689954134696,0.00006546077477083126,0.00006201363441313854,0.00005882588522482806,0.00005565941552704599,0.00005268566059673029,0.00004985389063939066,0.00004711054161988969,0.00004459962556585422,0.00004206326278212026,0.00003971750720141131,0.00003944991726419140,0.00003705800247276084,0.00003478518815679635,0.00003249953515937355,0.00002941752491872438,0.00002867605898832945,0.00002683614911682555,0.00002513639358366306,0.00002360319023303314,0.00002204730065244241,0.00002075431740336101,0.00001934546173573375,0.00001978825574950188,0.00001881965000913785,0.00001592762479605096,0.00001483524177390970,0.00001527592946995432,0.00001293159686433228,0.00001290481750472909,0.00001289502350793764,0.00001209173932021392,0.00001127824616251347,0.00000920756180646078,0.00000871811627972476,0.00000909263406892822,0.00000756755086921287,0.00000808361904494264,0.00000669703272801721};

// Auto-generated in MatLab at 30-May-2021 21:46:20 by Champ Darabundit
double OrganVoice::violinCoefs[65][5] = {
    {0.02527939788443444441, 0.00232930243930714546, -0.00480947005946959303, -1.66284175048162086163, 0.68564098074589285847},
    {0.02712233359950887504, 0.00255722119251794054, -0.00493863234561968574, -1.64936500382322215330, 0.67410592626962928531},
    {0.02910225197747995027, 0.00280189172803715870, -0.00506625196586143675, -1.63543445136885412516, 0.66227234310850979782},
    {0.03123590046843561022, 0.00305229282548436821, -0.00518538998756391898, -1.62104062858312447659, 0.65014343188948053864},
    {0.03343893585755588344, 0.00346301928736878319, -0.00537950148052667235, -1.60641541085376382547, 0.63793786451816181238},
    {0.03592476088759666980, 0.00370442766857926242, -0.00546381114757322605, -1.59097111683164094131, 0.62513649424024364532},
    {0.03847689781256893871, 0.00414658780665053562, -0.00564631698148088286, -1.57536609198498345208, 0.61234326062272204094},
    {0.04107279551830322201, 0.00484280770622995058, -0.00593921847272563773, -1.55951236088042888284, 0.59948874563223641942},
    {0.04422952579614641139, 0.00498704758082008093, -0.00591978384559842773, -1.54233751296843180079, 0.58563430249979986364},
    {0.04732906821776503709, 0.00558937107472627995, -0.00611466480034741564, -1.52518009568528789544, 0.57198387017743179683},
    {0.05054529162489769523, 0.00637957607759516519, -0.00644414460543576116, -1.50819075496284815685, 0.55867147805990524656},
    {0.05422654212083983377, 0.00686118480783422767, -0.00648849417706057283, -1.48936186977853113333, 0.54396110253014462455},
    {0.05803046070250765520, 0.00756796179795025947, -0.00667964208610242159, -1.47067152119215394102, 0.52959030160650943930},
    {0.06211416479763992582, 0.00827761026318604015, -0.00687436474346527805, -1.45169529853834755606, 0.51521270885570824571},
    {0.06628119251636697962, 0.00932466242941373293, -0.00722832091464538015, -1.43254703644760539305, 0.50092457047874072718},
    {0.07097450431473582844, 0.01007816087869225878, -0.00731523057645528611, -1.41180953505069961551, 0.48554696966767241229},
    {0.07554836624911408227, 0.01152401496446395918, -0.00777682107357276220, -1.39146486791084522672, 0.47076042805085049903},
    {0.08093233106186678572, 0.01228369701113172369, -0.00779304623771698811, -1.36953631361528893606, 0.45495929545057045562},
    {0.08611220004350324253, 0.01382058564863341069, -0.00842123579602822338, -1.34947470930311208548, 0.44098625919922052052},
    {0.09228803639622007449, 0.01460734291243873106, -0.00821839131533133420, -1.32527261837945764533, 0.42394960637278511495},
    {0.09822664139673117911, 0.01614574364259035652, -0.00884192343905184676, -1.30441953452579695316, 0.40994999612606664030},
    {0.10454669798586434248, 0.01801134162394922578, -0.00910126636635769093, -1.27961933764266988511, 0.39307611088612576244},
    {0.11167381941295412284, 0.01901404711074137360, -0.00937640458957971282, -1.25732527786491088406, 0.37863673979902667810},
    {0.11850288915747363150, 0.02119411709361543675, -0.01018259070334961471, -1.23458372435079577478, 0.36409813989853523353},
    {0.12599874672687341737, 0.02300557535024121483, -0.01082867896466212232, -1.21162032844569633738, 0.34979597155814884379},
    {0.13420433147127000528, 0.02455664560375401054, -0.01113918405867033411, -1.18679666659583205046, 0.33441845961218574779},
    {0.14236590013458311210, 0.02710819180583967647, -0.01170343182079758987, -1.16041553161494803703, 0.31818619173457324267},
    {0.15171040165894411622, 0.02777362448412985796, -0.01236611039439006708, -1.13956632703442539700, 0.30668424278310929543},
    {0.16106576155559104713, 0.02953774318928878428, -0.01310007683972162947, -1.11556806001434805076, 0.29307148791950626832},
    {0.17039885898829582045, 0.03232836186408251378, -0.01411297263385786864, -1.08982140171855323985, 0.27843564993707370370},
    {0.18096935077676906678, 0.03335586959993572242, -0.01493102001433907651, -1.06734333125912383977, 0.26673753162148955420},
    {0.19147701445043124879, 0.03386648550406463898, -0.01717427433055511779, -1.05200358155034190411, 0.26017280717428270531},
    {0.20278088228684049144, 0.03510033285776978373, -0.01827485376312566745, -1.02895516481678628296, 0.24856152619827087680},
    {0.21441924836981343661, 0.03206898660348387470, -0.02231655926903506823, -1.02726932489765854228, 0.25144100060192076107},
    {0.22628567187014081430, 0.03190533882300941593, -0.02490591485422524731, -1.01117840337104025750, 0.24446349920996524041},
    {0.23911565357339270577, 0.02073852025758456119, -0.03278326469305649360, -1.03800136126342557041, 0.26507227040134634377},
    {0.25220398831970952669, 0.00906591442483447446, -0.04075328712462703007, -1.06190065568890457115, 0.28241727130882154917},
    {0.26153191452645868642, 0.37909719854172391207, 0.11963228197381010898, 0.36338333488449980901, -0.60312193984250728196},
    {0.27557078106079962998, 0.39808768552645079630, 0.12280596182592601329, 0.38780542731501294051, -0.59134099890183633441},
    {0.29040994411097653671, 0.41551662933666488886, 0.12548786728009028724, 0.40581407926463664015, -0.57439963853690478857},
    {0.30649748337332954895, 0.28468496516657459949, 0.06710031332631913314, -0.05969409852240444014, -0.28202313961137231990},
    {0.31731263585592078824, -0.16835704669834647418, -0.12503408380206471051, -1.48919150221487339714, 0.51311300757038302844},
    {0.33785309379123795726, 0.46709808189490242825, 0.13196326156900897719, 0.45307504551184840658, -0.51616060825669907164},
    {0.35445655065421294294, 0.49234728071713895758, 0.13801203924071239748, 0.49037955992261061944, -0.50556368931054629368},
    {0.37114901563352459268, 0.51164811737894266841, 0.14049976428488916702, 0.50917171476880673797, -0.48587481747145022659},
    {0.38808269266382916518, 0.51696396117251153246, 0.13571740933091236414, 0.49222853782023306968, -0.45146447465297995238},
    {0.40556336846456708667, 0.55014840067148984559, 0.14484223715140337463, 0.54581295229744908326, -0.44525894600998888739},
    {0.42290912412651593399, 0.52806022961451226649, 0.12774614578470006032, 0.46787726577809823914, -0.38916176625237014486},
    {0.44045033118280918494, 0.58504111503010036710, 0.14629577894623702061, 0.57366116986130155642, -0.40187394470215481723},
    {0.45101495410212510961, -0.08785489496331118364, -0.09535754734965005974, -0.91851703125348105328, 0.18631954304264491951},
    {0.46712050368057672634, -0.11462034686658673088, -0.10418111493236981158, -0.94014637798457145745, 0.18846541986619161357},
    {0.49479196466662450637, 0.60515139637988779597, 0.13384995800956150935, 0.55709086623229486790, -0.32329754717622094518},
    {0.51106509480359274544, 0.66477291242748814959, 0.15327935194868486102, 0.65231351149904681641, -0.32319615231928100485},
    {0.52186680289566422797, 0.01633216512556775329, -0.07135590400431965352, -0.58092203453422164294, 0.04776509855113402619},
    {0.54907912519392043649, 0.70006763589315046392, 0.15449798467747344333, 0.68804782000514130846, -0.28440307424059674268},
    {0.56073443978706605240, 0.28642703379671696773, 0.00202218713307001068, -0.04818112762406680238, -0.10263521165908022492},
    {0.57108297154342102520, -0.05006352179657253681, -0.09429384036864114915, -0.61951216240893369758, 0.04623777178714100211},
    {0.58786148322006559486, -0.03744646085667066582, -0.09315964603533775923, -0.57043167417905304983, 0.02768705050711024393},
    {0.60507771362396611803, -0.00250995125960466183, -0.08666812147059750615, -0.48639430154407764517, 0.00229394243784163251},
    {0.63615283397341571003, 0.79176148081443498139, 0.15819060604479032173, 0.78374064845100699372, -0.19763572761836628588},
    {0.65038345381757434183, 0.77502014651237594034, 0.14123691719979680759, 0.75103628855170967427, -0.18439577102196250125},
    {0.65561038472408661448, 0.15159344391828369347, -0.05686605758321233717, -0.18188039134924416373, -0.06778183759159780997},
    {0.68230201018689029535, 0.82775045645777778347, 0.15108705978348496712, 0.81520556099265761851, -0.15406603456450471135},
    {0.68025653332110858074, -0.02315923577420162319, -0.10170863372240396272, -0.40475727054592330889, -0.03985406562957369281},
    {0.71055445730145250227, 0.82088183545870074820, 0.13252247269964309573, 0.79822081375669884373, -0.13426204829690233100}};