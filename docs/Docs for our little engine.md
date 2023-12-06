# 更新信息
12.6 zly
删除了所有的 Systems 的 `Init` 函数，将相关的逻辑移至其构造函数。
在 `Coordinator`、`SystemManager` 中增加了 `DestorySystem` 函数。

12.3 zly

- 概要
  - 引入了 `Engine` 类：`Engine` 即为字面意义上的 “引擎”，提供了三个主要的成员函数 `Init、Run `和  `Shutdown`，包含引擎的完整生命周期。
    - `Init`：初始化 `Coordinator` 并注册 Systems。
    - `Run`：包含一个 while 循环，每一次 loop 相当于一帧，`Engine` 会计算出每一帧的 delta time，调用每个 System 的 `Update` 函数来更新相关数据。
    - `RunEx`：`Run` 的变体，仅用于 Editor 中，支持在前面提到的 while 循环中执行自定义函数。
    - `Shutdown`：释放引擎运行时所分配的资源（目前由于所有的资源都受到智能指针的管理，故此函数暂时留空）。

  - System  `Update` 优先级的控制：前面提到，各个 System 在每一帧里中会被 `Update`，但原先的 ecs 架构并未对 `Update` 的顺序做明确的规定。按照我们的常识，如果想让最新的结果被呈现在屏幕上，应当把逻辑部分（logic），比如计算各种 GameObject 的位置、姿态等等，和 渲染部分（render）分开，而不能让它们互相穿插，为此就需要根据各个 System 的性质（属于 logic 还是 render）规定它们的 `Update` 顺序，或者说给它们增加优先级（priority）。因此我做的是：
    - 给 `System` 类增加了一个纯虚函数 `int GetPriority()`，强制要求每个 System 实现此函数，返回它们的优先级。为了方便，我增加了一个文件 EngineSettings.h 用于存放优先级数据。
    - 利用 `std::map` 来实现基于优先级的 `Update`（也可以采用 vector 实现）。同时，因为 System 是非内置类型，不支持 `<` 的比较操作，因此引入了 `SystemWrapper` 类对System 的优先级及其裸指针进行了封装，并重载了 `<` 运算符，这样就可以写出形如 `std::map<xxx, SystemWrapper>` 的代码了。
      - 如果你问为什么不用 `std::set`，因为它的迭代器默认具有 `const` 属性，因此遍历的时候没法调用 `Update` 这个非 `const` 成员函数，而 `std::map` 可以。

  - 引入了 `RenderSystem` 类：对应的是 glad 库，负责各种渲染操作。目前的方案是先将引擎的画面渲染至纹理（参考 LearnOpenGL 的 “帧缓冲” ），再根据需要将其显示在屏幕上或是 Editor 窗口内。这么做主要是因为引擎的画面需要集成到 Editor 里面，若采用调整 viewport 的方法，则需要根据 Editor 所设置的 Engine Window 的尺寸和坐标，对 viewport 的参数进行动态调整，有点麻烦，不如直接渲染至纹理，然后利用 ImGui 的 Image 控件把它贴到 Engine Window 或者是别的什么地方上。
  
  - 引入了 `WindowSystem` 类：对应的是 glfw 库，负责和窗口、交互有关的操作，比如注册各种回调函数，交换前后缓冲区，poll event 等等。

- 已知问题  
  CMake：cmake 使用 file glob 命令来自动获取文件夹下的所有头文件和源文件，但是 “获取” 操作只会在重新生成 cmake 缓存（cmake -S ./ -B build）的时候进行。因此，有时候会出现这么一种情况，就是你新建了某个文件，然后构建项目（cmake --build build），发现这个文件没被编译，导致报了一堆 link error，这种情况下需要重新跑一次 cmake -S ./ -B build 命令把新创建的文件囊括到 cmake 的管理下面。

  PS: 如果用的是 vscode 的话，打开任意 CMakeList.txt 然后按 CTRL+S 即可重新生成 cmake 缓存。

11.29 yyz

现在都改成ecs架构了，以后应该只要写components和systems了吧

后面慢慢把gameobjects迁移过去

11.28 yyz
新写了一个GameObject的基类，引擎可以从这里开始写，比如Mesh Material Texture这三个类

不过还有几个复杂度的问题，后面会改掉，还有后面会加一版不存ID直接存指针的结构，应该会快一点



# 文档

### 1.GameObject

 `Class GameObject`是我设想中的所有引擎中的可绘制物体的基类，我们可以更改这个类的属性来改变绘制相关的属性，以后可以往这里面加东西改东西，看我们的需求。

```cpp
int g_minAvailuableID = 0;
//最小的可以使用的ID，id生成时+1

std::map<int, GameObject *> g_gameObjects;
//储存了所有GameObject，是int和GameObject*的键值对

std::multimap<int, int> g_disableZBufferList;
//不使用ZBuffer的RenderingList，先于g_enableZBufferList被渲染，以保证显示在其之上
//是按照SortingOrder从小到大排列的
//这样遍历时，就会先绘制SortingOrder大的，再绘制SortingOrder小的，这样就可以保证显示顺序

std::multimap<int, int> g_enableZBufferList;
//使用ZBuffer的RenderingList

class GameObject
{
public:
    bool IsHidingFlag = false;
    //如果IsHidingFlag是true，该GameObject将不会被引擎绘制
    
    int Tag = DEFAULT_TAG;
    //作为碰撞发生时的标识符，比如我们只想选中摄像机时，我们可以检测碰撞时加一句tag==CAMERA_TAG
    
    int32_t Layer = 0b0;
    //32位int是32个独立的标签，我们在碰撞时可以设置ignoreLayer，位运算为1时再处理该碰撞
    //https://docs.unity3d.com/cn/2020.2/ScriptReference/LayerMask.html
    //仅仅检测位掩码为1的碰撞
    
    std::string Name = "GameObject";
    //物体名字

    GameObject();
    //构造函数，构造后将把GameObject加入g_gameObjects和g_enableZBufferList（默认使用ZBuffer）
    GameObject(std::string name, Mesh *mesh, Texture *texture, Material *material);
    //用给定的name mesh texture material构造一个GameObject
    ~GameObject();
    //析构函数，从g_gameObjects，g_enableZBufferList，g_disableZBufferList里删除

    bool GetUsingZBufferFlag();
    //获取ZBuffer Flag
    
    int GetSortingOrder();
    //获取SortingOrder
    
    int GetID();
    //获取ID

    int GetChildCount();
    //获取子物体数量

    GameObject *GetParent();
    //获取父物体

    GameObject *GetFirstChild();
    //获取第一个子物体

    GameObject *GetPrevSibling();
    //获取上一个兄弟物体

    GameObject *GetNextSibling();
    //获取下一个兄弟物体

    glm::vec3 GetPosition();
    //获取位置

    glm::vec3 GetRotation();
    //获取旋转

    glm::vec3 GetScale();
    //获取缩放
    
    void SetZBuffer(bool isUsingZBufferFlag);
    //设置是否使用ZBuffer，设置时会自动调整g_enableZBufferList和g_disableZBufferList
    //如果设置为true，将会从g_disableZBufferList中删除，加入g_enableZBufferList
    //如果设置为false，将会从g_enableZBufferList中删除，加入g_disableZBufferList

    void SetSortingOrder(int sortingOrder);
    //设置SortingOrder，设置时会自动调整g_enableZBufferList使其有序

    void AssignID();
    //分配ID，会让g_minAvailuableID+1，然后把ID赋值给GameObject

    void SetParent(GameObject *parent, int index = 0);
    //设置父物体，index为设置后子物体的位置，如果index超过了子物体数量，将会设置为最后一个子物体的后面
    //如果父物体是子物体的子物体，会直接返回

    void RemoveParent();
    //移除父物体，如果没有父物体，会直接返回

    void SetPosition(glm::vec3 position);
    //设置位置，会自动更新modelMatrix

    void SetRotation(glm::vec3 rotation);
    //设置旋转，会自动更新modelMatrix

    void SetScale(glm::vec3 scale);
    //设置缩放，会自动更新modelMatrix

    bool operator==(const GameObject &other) const;
    //重载==，用于比较两个GameObject是否相等，如果ID相等，就相等

private:
    bool _isUsingZBufferFlag = false; 
    //是否使用ZBuffer，如果为false，将会被加入g_disableZBufferList，否则加入g_enableZBufferList

    int _sortingOrder = 0;
    //排序顺序，用于控制绘制顺序，SortingOrder越大，越早绘制

    int _id = -1;
    //ID，用于标识GameObject，每个GameObject都有唯一的ID

    int _childCount;
    //子物体数量

    int _parentID = -1;
    //父物体ID，如果没有父物体，为-1

    int _firstChildrenID = -1;
    //第一个子物体ID，如果没有子物体，为-1

    int _prevSiblingID = -1;
    //上一个兄弟物体ID，如果没有上一个兄弟物体，为-1

    int _nextSiblingID = -1;
    //下一个兄弟物体ID，如果没有下一个兄弟物体，为-1

    glm::vec3 _position = glm::vec3(0.0f);
    //位置

    glm::vec3 _rotation = glm::vec3(0.0f);
    //旋转 欧拉角 度数

    glm::vec3 _scale = glm::vec3(1.0f);
    //缩放

    glm::mat4 _translateMatrix = glm::mat4(1.0f);
    glm::mat4 _rotateMatrix = glm::mat4(1.0f);
    glm::mat4 _scaleMatrix = glm::mat4(1.0f);
    //分别是平移矩阵，旋转矩阵，缩放矩阵
    
    glm::mat4 _modelMatrix = glm::mat4(1.0f);
    //模型矩阵，可以被传入shader

    // TODO: empty, 因为我还不太清楚modern opengl的渲染流程
    Mesh *_mesh;
    Texture *_texture;
    Material *_material;
    // ...
};
```



