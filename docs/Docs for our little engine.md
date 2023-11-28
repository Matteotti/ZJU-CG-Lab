# 更新信息

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

    glm::mat4 _modelMatrix = glm::mat4(1.0f);
    //模型矩阵，可以被传入shader

    // TODO: empty, 因为我还不太清楚modern opengl的渲染流程
    Mesh *_mesh;
    Texture *_texture;
    Material *_material;
    // ...
};
```



