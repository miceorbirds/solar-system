//Include and link appropriate libraries and headers//
#include <Windows.h>
#include <iostream>
#include "winuser.h"
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>
#include <dcomp.h>
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

//Global Declarations - Interfaces//
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;
ID3D11Buffer* squareIndexBuffer;
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;
ID3D11Buffer* squareVertBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* vertLayout;
ID3D11Buffer* cbPerObjectBuffer;

//Global Declarations - Others//
LPCTSTR WndClassName = L"firstwindow";
HWND hwnd = NULL;
HRESULT hr;

const int Width = 900;
const int Height = 900;

DirectX::XMMATRIX WVP;

XMMATRIX c_mercury_world;
XMMATRIX c_sun_world;
XMMATRIX c_venus_world;
XMMATRIX c_Earth_world;
XMMATRIX c_Mars_world;
XMMATRIX c_Upiter_world;
XMMATRIX c_Saturn_world;
XMMATRIX c_Uran_world;
XMMATRIX c_Neptune_world;
XMMATRIX c_Pluto_world;
XMMATRIX c_moon_world;
XMMATRIX c_io_world;
XMMATRIX c_europe_world;
XMMATRIX c_ganimed_world;

XMMATRIX camView;
XMMATRIX camProjection;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;
float rot = 0.01f;

float rot_merc = 0.01f;
float rot_venus = 0.01f;
float rot_earth = 0.01f;
float rot_mars = 0.01f;
float rot_upiter = 0.01f;
float rot_saturn = 0.01f;
float rot_uran = 0.01f;
float rot_neptune = 0.01f;
float rot_pluto = 0.01f;
float rot_moon = 0.01f;
float rot_io = 0.01f;
float rot_europe = 0.01f;
float rot_ganimed = 0.01f;

float rot_merc_self = 0.01f;
float rot_venus_self = 0.01f;
float rot_earth_self = 0.01f;
float rot_mars_self = 0.01f;
float rot_upiter_self = 0.01f;
float rot_saturn_self = 0.01f;
float rot_uran_self = 0.01f;
float rot_neptune_self = 0.01f;
float rot_pluto_self = 0.01f;
float rot_moon_self = 0.01f;




//Function Prototypes//
bool InitializeDirect3d11App(HINSTANCE hInstance);
void CleanUp();
bool InitScene();
void UpdateScene();
void DrawScene();

bool InitializeWindow(HINSTANCE hInstance,
    int ShowWnd,
    int width, int height,
    bool windowed);
int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

//Create effects constant buffer's structure//
struct cbPerObject
{
    XMMATRIX  WVP;
};

cbPerObject cbPerObj;

//Vertex Structure and Vertex Layout (Input Layout)//
struct Vertex    //Overloaded Vertex Structure
{
    struct
    {
        float x;
        float y;
        float z;
    } pos;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

UINT numElements = ARRAYSIZE(layout);

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
    {
        MessageBox(0, L"Window Initialization - Failed",
            L"Error", MB_OK);
        return 0;
    }

    if (!InitializeDirect3d11App(hInstance))    //Initialize Direct3D
    {
        MessageBox(0, L"Direct3D Initialization - Failed",
            L"Error", MB_OK);
        return 0;
    }

    if (!InitScene())    //Initialize our scene
    {
        MessageBox(0, L"Scene Initialization - Failed",
            L"Error", MB_OK);
        return 0;
    }

    messageloop();

    CleanUp();

    return 0;
}

bool InitializeWindow(HINSTANCE hInstance,
    int ShowWnd,
    int width, int height,
    bool windowed)
{
    typedef struct _WNDCLASS {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HANDLE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCTSTR lpszMenuName;
        LPCTSTR lpszClassName;
    } WNDCLASS;

    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WndClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Error registering class",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    hwnd = CreateWindowEx(
        NULL,
        WndClassName,
        L"E pur si muove!",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd)
    {
        MessageBox(NULL, L"Error creating window",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, ShowWnd);
    UpdateWindow(hwnd);

    return true;
}

bool InitializeDirect3d11App(HINSTANCE hInstance)
{
    //Describe our SwapChain Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = Width;
    bufferDesc.Height = Height;
    bufferDesc.RefreshRate.Numerator = 0;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //Create our SwapChain
    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

    //Create our BackBuffer
    ID3D11Texture2D* BackBuffer;
    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

    //Create our Render Target
    hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
    BackBuffer->Release();

    //Describe our Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = Width;
    depthStencilDesc.Height = Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    //Create the Depth/Stencil View
    d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
    d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

    //Set our Render Target
    d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    return true;
}

void CleanUp()
{
    //Release the COM Objects we created
    SwapChain->Release();
    d3d11Device->Release();
    d3d11DevCon->Release();
    renderTargetView->Release();
    squareVertBuffer->Release();
    squareIndexBuffer->Release();
    VS->Release();
    PS->Release();
    VS_Buffer->Release();
    PS_Buffer->Release();
    vertLayout->Release();
    depthStencilView->Release();
    depthStencilBuffer->Release();
    cbPerObjectBuffer->Release();
}

bool InitScene()
{
    //Compile Shaders from shader file
    ID3DBlob* errorVertexCode;
    ID3DBlob* errorPixelCode;
	
    hr = D3DCompileFromFile(L"MiniTri.fx",
        nullptr ,
        nullptr ,
        "VS",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &VS_Buffer,
        &errorVertexCode);

    if (FAILED(hr)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
        }

        return false;
    }
	
    hr = D3DCompileFromFile(L"MiniTri.fx",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &PS_Buffer,
        &errorPixelCode);

    if (FAILED(hr)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorPixelCode) {
            char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
        }

        return false;
    }
	
    if (FAILED(hr))
    {

    }

    //Create the Shader Objects
    hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
    hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

    //Set Vertex and Pixel Shaders
    d3d11DevCon->VSSetShader(VS, 0, 0);
    d3d11DevCon->PSSetShader(PS, 0, 0);

    //Create the vertex buffer
    Vertex v[] =
    {
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
    };

    DWORD indices[] = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = indices;
    d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);

    d3d11DevCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * 8;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;


    D3D11_SUBRESOURCE_DATA vertexBufferData;

    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = v;
    hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

    //Set the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

    //Create the Input Layout
    hr = d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
         VS_Buffer->GetBufferSize(), &vertLayout);

    //Set the Input Layout
    d3d11DevCon->IASetInputLayout(vertLayout);

    //Set Primitive Topology
    d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Create the Viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    //Set the Viewport
    d3d11DevCon->RSSetViewports(1, &viewport);

    //Create the constant buffer
    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(cbPerObject);
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;

    hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

    // lookup table for cube face colors
    struct ConstantBuffer2
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        } face_colors[6];
    };
    const ConstantBuffer2 cb2 =
    {
        {
            {1.0f,0.0f,1.0f},
            {1.0f,0.0f,0.0f},
            {0.0f,1.0f,0.0f},
            {0.0f,0.0f,1.0f},
            {1.0f,1.0f,0.0f},
            {0.0f,1.0f,1.0f},
        }
    };
    ID3D11Buffer* pConstantBuffer2;
    D3D11_BUFFER_DESC cbd2;
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd2.Usage = D3D11_USAGE_DEFAULT;
    cbd2.CPUAccessFlags = 0u;
    cbd2.MiscFlags = 0u;
    cbd2.ByteWidth = sizeof(cb2);
    cbd2.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd2 = {};
    csd2.pSysMem = &cb2;
    d3d11Device->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);

    // bind constant buffer to pixel shader
    d3d11DevCon->PSSetConstantBuffers(0u, 1u, &pConstantBuffer2);


    //Camera information
    camPosition = XMVectorSet(0.0f, 20.0f, -50.0f, 0.0f);
    camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //Set the View matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

    //Set the Projection matrix
    camProjection = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)Width / Height, 1.0f, 1000.0f);

    return true;
}

void UpdateScene()
{
    //Keep the cubes rotating
    rot += .0009f;
    if (rot > 6.26f)
        rot = 0.0f;


    XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR rotaxis2 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //SUN
    c_sun_world = XMMatrixIdentity(); // обнуляем матрицу
    Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    c_sun_world = Translation * Rotation * Scale;

    //MERCURY
    rot_merc += .003;
    if (rot_merc > 6.26f)
        rot_merc = 0.0f;
    rot_merc_self += .00001407;
    if (rot_merc_self > 6.26f)
        rot_merc_self = 0.0f;
    c_mercury_world = XMMatrixIdentity();
    Translation = XMMatrixTranslation(0.0f, 0.0f, 7.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_merc);
    Scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
    XMMATRIX SelfRotation_merc = XMMatrixRotationAxis(rotaxis, -rot_merc_self);
    c_mercury_world = SelfRotation_merc * Translation * Rotation * Scale; // IT WORKS!

    //VENUS
    rot_venus += .002;
    if (rot_venus > 6.26f)
        rot_venus = 0.0f;
    rot_venus_self += .000005832;
    if (rot_venus_self > 6.26f)
        rot_venus_self = 0.0f;
    c_venus_world = XMMatrixIdentity();
    Translation = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
    Rotation = XMMatrixRotationAxis(rotaxis2, -rot_venus);
    Scale = XMMatrixScaling(0.4f, 0.4f, 0.4f);
    XMMATRIX SelfRotation_venus = XMMatrixRotationAxis(rotaxis, rot_venus_self);
    c_venus_world = SelfRotation_venus * Translation * Rotation * Scale; // IT WORKS!

    //EARTH
    rot_earth += .001;
    if (rot_earth > 6.26f)
        rot_earth = 0.0f;
    rot_earth_self += .0023;
    if (rot_earth_self > 6.26f)
        rot_earth_self = 0.0f;
    c_Earth_world = XMMatrixIdentity();
    Translation = XMMatrixTranslation(0.0f, 0.0f, 8.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_earth);
    Scale = XMMatrixScaling(0.8f, 0.8f, 0.8f);
    XMMATRIX earth = Translation * Rotation * Scale;
    XMMATRIX SelfRotation_earth = XMMatrixRotationAxis(rotaxis, -rot_earth_self);
    c_Earth_world = SelfRotation_earth * Translation * Rotation * Scale;

    //Moon
    rot_moon += .012;
    if (rot_moon > 6.26f)
        rot_moon = 0.0f;
    c_moon_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_moon);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 15.0f);
    Scale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
    c_moon_world = Translation * Rotation * Scale * earth;

    //MARS
    rot_mars += .0005;
    if (rot_mars > 6.26f)
        rot_mars = 0.0f;
    rot_mars_self += .0024;
    if (rot_mars_self > 6.26f)
        rot_mars_self = 0.0f;
    c_Mars_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_mars);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
    Scale = XMMatrixScaling(1.1f, 1.1f, 1.1f);
    XMMATRIX SelfRotation_mars = XMMatrixRotationAxis(rotaxis, -rot_mars_self);
    c_Mars_world = SelfRotation_mars * Translation * Rotation * Scale;

    //UPITER
    rot_upiter += .000083;
    if (rot_upiter > 6.26f)
        rot_upiter = 0.0f;
    rot_upiter_self += .0099;
    if (rot_upiter_self > 6.26f)
        rot_upiter_self = 0.0f;
    c_Upiter_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_upiter);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 15.0f);
    Scale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
    XMMATRIX upiter = Translation * Rotation * Scale;
    XMMATRIX SelfRotation_upiter = XMMatrixRotationAxis(rotaxis, -rot_upiter_self);
    c_Upiter_world = SelfRotation_upiter * Translation * Rotation * Scale;

    //io
    rot_io += .008;
    if (rot_io > 6.26f)
        rot_io = 0.0f;
    c_io_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_io);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 15.0f);
    Scale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
    c_io_world = Translation * Rotation * Scale * upiter;
    //europe
    rot_europe += .004;
    if (rot_europe > 6.26f)
        rot_europe = 0.0f;
    c_europe_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_europe);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 20.0f);
    Scale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
    c_europe_world = Translation * Rotation * Scale * upiter;
    //ganimed
    rot_ganimed += .002;
    if (rot_ganimed > 6.26f)
        rot_ganimed = 0.0f;
    c_ganimed_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_ganimed);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 25.0f);
    Scale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
    c_ganimed_world = Translation * Rotation * Scale * upiter;

    //Saturn
    rot_saturn += .000034;
    if (rot_saturn > 6.26f)
        rot_saturn = 0.0f;
    rot_saturn_self += .0010;
    if (rot_saturn_self > 6.26f)
        rot_saturn_self = 0.0f;
    c_Saturn_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_saturn);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 20.0f);
    Scale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
    XMMATRIX SelfRotation_saturn = XMMatrixRotationAxis(rotaxis, -rot_upiter_self);
    c_Saturn_world = SelfRotation_saturn * Translation * Rotation * Scale;

    //Uran
    rot_uran += .0000119;
    if (rot_uran > 6.26f)
        rot_uran = 0.0f;
    rot_uran_self += .0017;
    if (rot_uran_self > 6.26f)
        rot_uran_self = 0.0f;
    c_Uran_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_uran);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 25.0f);
    Scale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
    XMMATRIX SelfRotation_uran = XMMatrixRotationAxis(rotaxis, -rot_uran_self);
    c_Uran_world = SelfRotation_uran * Translation * Rotation * Scale;

    //Neptune
    rot_neptune += .00000613;
    if (rot_neptune > 6.26f)
        rot_neptune = 0.0f;
    rot_neptune_self += .0016;
    if (rot_neptune_self > 6.26f)
        rot_neptune_self = 0.0f;
    c_Neptune_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_neptune);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 35.0f);
    Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    XMMATRIX SelfRotation_neptune = XMMatrixRotationAxis(rotaxis, -rot_neptune_self);
    c_Neptune_world = SelfRotation_neptune * Translation * Rotation * Scale;

    //Pluto
    rot_pluto += .00000401;
    if (rot_pluto > 6.26f)
        rot_pluto = 0.0f;
    rot_pluto_self += .000153;
    if (rot_pluto_self > 6.26f)
        rot_pluto_self = 0.0f;
    c_Pluto_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot_pluto);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 80.0f);
    Scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
    XMMATRIX SelfRotation_pluto = XMMatrixRotationAxis(rotaxis, -rot_pluto_self);
    c_Pluto_world = SelfRotation_pluto * Translation * Rotation * Scale;
}

void DrawScene()
{
    //Clear our backbuffer
    float bgColor[4] = { 0.827451050f,  0.827451050f, 0.827451050f, 1.0f };
    d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
    //Refresh the Depth/Stencil view
    d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

    //Draw the sun
    WVP = c_sun_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the mercury
    WVP = c_mercury_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Venus
    WVP = c_venus_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Earth
    WVP = c_Earth_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Moon
    WVP = c_moon_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Mars
    WVP = c_Mars_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Upiter
    WVP = c_Upiter_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the io
    WVP = c_io_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Europe
    WVP = c_europe_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the ganimed
    WVP = c_ganimed_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Saturn
    WVP = c_Saturn_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Uran
    WVP = c_Uran_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Neptune
    WVP = c_Neptune_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);
    
    //Draw the Pluto
    WVP = c_Pluto_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Present the backbuffer to the screen
    SwapChain->Present(0, 0);
}

int messageloop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (true)
    {
        BOOL PeekMessageL(
            LPMSG lpMsg,
            HWND hWnd,
            UINT wMsgFilterMin,
            UINT wMsgFilterMax,
            UINT wRemoveMsg
        );

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // run game code            
            UpdateScene();
            DrawScene(); 
        }
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd,
        msg,
        wParam,
        lParam);
}